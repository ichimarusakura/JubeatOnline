//***********************************************
//*		ImageSequence Class Source File			*
//*			Programmed by white					*
//***********************************************


#include <string.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <exception>

#include "image_sequence.hpp"
#include "output_logtext.hpp"

jubeat_online::ImageSequence::ImageSequence() {
	filename_ = NULL;
	images_ = NULL;

	all_image_frame_ = 0;
	now_frame_ = 0;
	fps_ = 30;

	is_repeat_ = 0;
	in_frame_ = 0;
	out_frame_ = 0;

	x_ = 0;
	y_ = 0;
	exrate_ = 1.0f;
	is_play_ = false;

	is_allocated_ = false;
	is_loaded_ = false;

	load_result_ = ImageSequenceResult::LOAD_ERROR;

}

jubeat_online::ImageSequence::~ImageSequence() {
	DeleteSequence();
}


void jubeat_online::ImageSequence::LoadThread(void) {

	//***********************************************************************
	//関数名:(private)LoadThread
	//説　明:別スレッドで連番画像の読み込みをします
	//戻り値:void型。結果はload_result_に格納されます。
	//***********************************************************************
	
	std::lock_guard<std::mutex> lock(mtx_);

	//戻り値格納用
	ImageSequenceResult ret = ImageSequenceResult::OK;
	std::string a;

	//シーケンス画像を読み込む
	//ファイルを開封
	std::ifstream fp(filename_, std::ios::binary);
	//mtx.lock();
	if (fp.fail()) {
		//ファイルの開封に失敗
		ret = ImageSequenceResult::LOAD_ERROR;
		a = "ファイルの開封に失敗しました:";
		a += filename_;
		jubeat_online::OutputLogtext::OutputS(a,"ImageSequence");
		return;
	}

	char type = 0x00, pass = 0x00, length = 0x00, fps = 0x00;
	char tm[2];

	do {

		//まず識別子の取得
		fp.read(&type, 1);
		fp.read(&pass, 1);
		fp.read(&length, 1);
		fp.read(&fps, 1);
		fp.read(tm, 2);

		if (fp.bad()) {
			OutputLogtext::Output("ファイル形式が間違っています","ImageSequence");
			ret = ImageSequenceResult::MALFORMED_FILE;
		}

		//総フレーム数
		all_image_frame_ = 0;
		for (int i = 0; i < 2; i++) {
			unsigned int t = static_cast<unsigned int>(0x000000ff & tm[i]);
			t <<= 8 * i;
			all_image_frame_ |= t;
		}

		a = filename_;
		a += ":総ファイル数:";
		a += std::to_string(all_image_frame_);

		OutputLogtext::OutputS(a, "ImageSequence");

		char* size_str = new char[length];

		//画像フレーム格納領域
		images_ = new sf::Texture[all_image_frame_];
		if (images_ == NULL) {
			ret = ImageSequenceResult::OUT_OF_MEMORY;
			break;
		}
		else do {

			unsigned int loaded = 0;	//読み込みが完了したフレーム数
			for (loaded = 0; loaded < all_image_frame_ && ret == ImageSequenceResult::OK; loaded++) {


				//ループに入った後失敗した場合、画像のメモリ解放もしなければならない
				//読み込まれた画像はloaded枚となる

				//ファイル長を取得
				long size = 0;

				if (size_str == NULL) {
					ret = ImageSequenceResult::OUT_OF_MEMORY;
					break;
				}
				else do {

					//読み込み（サイズ）
					fp.read(size_str, length);
					if (fp.bad()) {
						a = "ファイル形式が不正です:サイズヘッダ読み込み:";
						a += filename_;
						OutputLogtext::OutputS(a, "ImageSequence");
						ret = ImageSequenceResult::MALFORMED_FILE;
						break;
					}

					for (int i = 0; i < length; i++) {
						long t = static_cast<long>(0x000000ff & size_str[i]);
						t <<= 8 * i;
						size |= t;
					}

					//データ本体を持ってくる領域確保
					char* ndata = new char[size];

					if (ndata == NULL) {
						a = "メモリの確保に失敗しました:";
						a += filename_;
						OutputLogtext::OutputS(a, "ImageSequence");
						ret = ImageSequenceResult::OUT_OF_MEMORY;
						break;
					}
					else do {

						//ファイルを読み取る
						fp.read(ndata, size);
						if (fp.bad()) {
							a = "ファイル形式が不正です:データが不足しています:";
							a += filename_;
							OutputLogtext::OutputS(a, "ImageSequence");
							ret = ImageSequenceResult::MALFORMED_FILE;
							break;
						}

						for (int i = 0; i < size; i++) {
							//解凍作業
							ndata[i] ^= pass;
						}

						try {
							if (!images_[loaded].loadFromMemory(ndata, size)) {
								throw;
							}
						}
						catch (std::exception &ex) {
							a = "ファイルの読み込みに失敗しました:";
							a += filename_;
							a += ":";
							a += ex.what();
							OutputLogtext::OutputS(a, "ImageSequence");
						}

					} while (0);
					delete[] ndata;


				} while (0);


			}//end of for


			if (ret != ImageSequenceResult::OK) {
				//エラーが発生した場合の読み込んだ画像に対する開放処理
				delete[] images_;
			}

			fp.close();

		} while (0);


		delete[] size_str;
		size_str = NULL;

	} while (0);


	if (ret == ImageSequenceResult::OK) {
		is_loaded_ = true;
		out_frame_ = all_image_frame_;
	}

	load_result_ = ret;
	a = "読み込み処理が完了しました。スレッドを終了します:";
	a += filename_;
	OutputLogtext::OutputS(a, "ImageSequence");


	//mtx.unlock();
}

jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::LoadSequence(const char * filename){

	//***********************************************************************
	//関数名:LoadSeqeunce
	//説　明:独自形式の連続版画像の読み込み命令を発生させます
	//		 この関数が呼ばれるまでに、DxLib::SetUseASyncLoadFlag(TRUE)が
	//		 呼ばれている必要があります。呼ばない場合はこの関数内で
	//		 完全にロードが完了します。
	//戻り値:ImageSequenceResult型
	//***********************************************************************

	ImageSequenceResult ret = ImageSequenceResult::OK;

	//シーケンス画像ファイルのファイル名の処理
	if (filename != NULL) {
		ImageSequenceResult fret = SetSequenceFilename(filename);
		if(fret != ImageSequenceResult::OK) return fret;
	}

	else if (filename_ == NULL) return ImageSequenceResult::NULL_FILEPATH;		//ファイル名を指定しろ
	

	if (is_allocated_) DeleteSequence();
	
	OutputLogtext::OutputS("読み込みスレッドを開始します:" + static_cast<std::string>(filename),"ImageSequence");

	std::thread th(&jubeat_online::ImageSequence::LoadThread, this);
	th.detach();

	is_allocated_ = true;

	return ret;
}

jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::SetSequenceFilename(const char * filename){

	//***********************************************************************
	//関数名:SetSequenceFilename
	//説　明:読み込むファイル名を設定します。
	//		 この関数でロードは発生しません。
	//戻り値:ImageSequenceResult型
	//***********************************************************************

	if (filename_ == NULL) {
		//初回の登録
		size_t fsize = strlen(filename) + 1;
		filename_ = static_cast<char*>(malloc(sizeof(char) * fsize));
		if (filename_ == NULL) return ImageSequenceResult::OUT_OF_MEMORY;
		strcpy_s(filename_, fsize, filename);
	}
	else {
		free(filename_);
		filename_ = NULL;
		return ImageSequence::SetSequenceFilename(filename);
	}

	return ImageSequenceResult::OK;
}

int jubeat_online::ImageSequence::WaitLoadComplete(void) {

	//***********************************************************************
	//関数名:WaitLoadComplete
	//説　明:ロードが終わったかどうか取得できます。
	//戻り値:int型で、0なら成功、正の値なら、まだ読み込めていないということ、
	//		 負の値なら読み込みに失敗した枚数分をマイナスで表示しています。
	//***********************************************************************

	//LoadSequence実行したか
	if (!is_allocated_) return -1;
	
	if (is_loaded_) {
		if(load_result_ == ImageSequenceResult::OK) return 0;
		else return -1;
	}
	return 1;
}


int jubeat_online::ImageSequence::DrawSequence(const float x, const float y, const float ex, sf::RenderTexture * screen_buffer)
{
	exrate_ = ex;
	return DrawSequence(x,y,screen_buffer);
}

int jubeat_online::ImageSequence::DrawSequence(const float x, const float y, sf::RenderTexture * screen_buffer)
{
	x_ = x;
	y_ = y;
	return DrawSequence(screen_buffer);
}

int jubeat_online::ImageSequence::DrawSequence(sf::RenderTexture* screen_buffer)
{
	//***********************************************************************
	//関数名:DrawSequence
	//説　明:画面に実際に描写します
	//戻り値:int型。-1は描写せず、それ以外は描写したフレーム番号
	//***********************************************************************
	//ロードは完了しているか
	if (is_loaded_ == false) return -1;

	//再生中か
	if (is_play_) {
		sf::Time t(started_time_.getElapsedTime());
		now_frame_ = static_cast<int>(t.asSeconds() * fps_);
		if (now_frame_ >= all_image_frame_ || now_frame_ >= out_frame_) {
			if (is_repeat_ == true) {
				//リピート処理
				now_frame_ %= (out_frame_ - in_frame_);
			}
			else {
				is_play_ = false;
				now_frame_ = 0;
			}
		}

		if (is_play_ == true) {

			sf::Sprite graph(images_[now_frame_]);
			graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
			graph.setPosition(x_, y_);
			graph.setScale(exrate_, exrate_);
			screen_buffer->draw(graph);
		}

		return now_frame_;
	}
	

	return -1;
}

int jubeat_online::ImageSequence::DrawFrame(const float x, const float y, const float ex, const unsigned int frame, sf::RenderTexture* screen_buffer) {

	if (frame >= 0 && frame < all_image_frame_) {

		sf::Sprite graph(images_[frame]);
		graph.setOrigin(graph.getLocalBounds().width / 2.0f, graph.getLocalBounds().height / 2.0f);
		graph.setPosition(x, y);
		graph.setScale(ex, ex);
		screen_buffer->draw(graph);
		return 0;
	}
	return -1;
}

void jubeat_online::ImageSequence::DeleteSequence(void){

	//***********************************************************************
	//関数名:DeleteSequence
	//説　明:シーケンスの削除をします。初期化と同様です。
	//***********************************************************************

	//シーケンスの初期化を行います。

	if (is_allocated_ == false) return;
	
	if (images_ != NULL) {
		delete[] images_;
		images_ = NULL;
	}
	

	if (filename_ != NULL) {
		delete[] filename_;
		filename_ = NULL;
	}

	all_image_frame_ = 0;
	now_frame_ = 0;
	fps_ = 30;

	is_repeat_ = 0;
	in_frame_ = 0;
	out_frame_ = 0;

	x_ = 0;
	y_ = 0;
	exrate_ = 1.0f;

	is_allocated_ = false;

	is_allocated_ = false;
	is_loaded_ = false;
	
}

void jubeat_online::ImageSequence::set_x(const float x)
{
	x_ = x;
}

float jubeat_online::ImageSequence::x(void) const
{
	return x_;
}

void jubeat_online::ImageSequence::set_y(const float y)
{
	y_ = y;
}

float jubeat_online::ImageSequence::y(void) const
{
	return y_;
}

void jubeat_online::ImageSequence::set_in_frame(const unsigned int frame)
{
	in_frame_ = frame;
}

unsigned int jubeat_online::ImageSequence::in_frame(void) const
{
	return in_frame_;
}

void jubeat_online::ImageSequence::set_out_frame(const unsigned int frame)
{
	out_frame_ = frame;
}

unsigned int jubeat_online::ImageSequence::out_frame(void) const
{
	return out_frame_;
}

void jubeat_online::ImageSequence::InitInPoint(void)
{
	set_in_frame(0);
}

void jubeat_online::ImageSequence::InitOutPoint(void)
{
	set_out_frame(all_image_frame_);
}

void jubeat_online::ImageSequence::LoadDivThread(const int x_div, const int y_div, const int width, const int height)
{
	std::lock_guard<std::mutex> lock(mtx_);

	load_result_ = ImageSequenceResult::OK;

	sf::Image gr;
	if (gr.loadFromFile(filename_) == false) {
		OutputLogtext::OutputS("画像ファイルを読み込めませんでした。:" + static_cast<std::string>(filename_), "ImageSequence");
		load_result_ = ImageSequenceResult::LOAD_ERROR;
		return;
	}

	images_ = new sf::Texture[all_image_frame_];
	if (images_ == NULL) {
		load_result_ = ImageSequenceResult::OUT_OF_MEMORY;
		return;
	}

	for (int x = 0; x < x_div; x++) {
		for (int y = 0; y < y_div; y++) {
			if (images_[x + y * x_div].loadFromImage(gr, sf::IntRect(x * width, y * height, width, height)) == false) {
				OutputLogtext::OutputS("画像ファイルを読み込めませんでした。:" + static_cast<std::string>(filename_), "ImageSequence");
			}
		}
	}

	is_loaded_ = true;
	out_frame_ = all_image_frame_;
	OutputLogtext::OutputS("読み込みのスレッドを終了します:" + static_cast<std::string>(filename_), "ImageSequence");
}

jubeat_online::ImageSequenceResult jubeat_online::ImageSequence::LoadDivGraph(const int all_framecount, const int x_div, const int y_div, const int width, const int height, const char * filename)
{

	ImageSequenceResult ret = ImageSequenceResult::OK;

	//シーケンス画像ファイルのファイル名の処理
	if (filename != NULL) {
		ImageSequenceResult fret = SetSequenceFilename(filename);
		if (fret != ImageSequenceResult::OK) return fret;
	}

	all_image_frame_ = all_framecount;
	

	OutputLogtext::OutputS("読み込みのスレッドを開始します:" + static_cast<std::string>(filename_), "ImageSequence");
	std::thread th(&jubeat_online::ImageSequence::LoadDivThread, this,x_div,y_div,width,height);
	th.detach();

	is_allocated_ = true;

	return ret;
}

jubeat_online::ImageSequenceResult	jubeat_online::ImageSequence::PlaySequence(const unsigned int frame){

	//***********************************************************************
	//関数名:PlaySequence
	//説　明:シーケンス画像を再生します。
	//		 LoadSequence、WaitLoadCompleteを呼び出し、ロードが完成し
	//		 ている必要があります。もしも失敗している画像があれば
	//		 同等のサイズでIMAGE_SEQUENCE_DEFAULT_IMAGE_COLORに指定された
	//		 色番号の四角で埋められます。
	//引　数:[frame]描画を開始するフレーム
	//戻り値:int型で、0なら成功、正の値なら、そのフレーム分がまだ読み込めて
	//		 いないということ、負の値なら読み込みに失敗した枚数分をマイナスで
	//		 表示しています。
	//***********************************************************************

	if (is_loaded_ == false) return ImageSequenceResult::LOAD_ERROR;


	now_frame_ = frame;

	is_play_ = true;
	started_frame_ = frame;
	started_time_.restart();

	
	return ImageSequenceResult::OK;
}

void jubeat_online::ImageSequence::set_is_repeat(const bool flag)
{
	is_repeat_ = flag;
}

bool jubeat_online::ImageSequence::is_repeat(void) const
{
	return is_repeat_;
}

void jubeat_online::ImageSequence::set_fps(const unsigned int fps)
{
	fps_ = fps;
}
