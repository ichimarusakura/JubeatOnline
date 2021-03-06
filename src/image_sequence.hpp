//***************************************************
//*   ImageSequence Class Library					*
//*													*
//*		Programmed by ほわいと						*
//***************************************************


#pragma once
#ifndef JUBEAT_ONLINE_IMAGESEQUENCE_H_
#define JUBEAT_ONLINE_IMAGESEQUENCE_H_

#include <mutex>

#include <SFML/Graphics.hpp>


namespace jubeat_online {

	//c_ImageSequence関数について
	//	シーケンス画像をもとにアニメーションを展開します。
	//	この関数で、シーケンス画像のロードから描画まですべて行います
	//	ただし、シーケンス画像を動画として再生するプログラムですので
	//	座標や表示状態などは外部から指定する必要があります。

	/// <summary>ImageSequenceクラスの、結果を戻り値とする関数の、戻り値の型</summary>
	enum ImageSequenceResult {
		OK = 0,
		LOAD_ERROR = -1,
		MALFORMED_FILE = -2,
		NULL_FILEPATH = -3,
		OUT_OF_MEMORY = -4,
	};

	/// <summary>シーケンス画像をもとにアニメーションを展開します。
	/// このクラスで、シーケンスの画像のロードから描画まですべて行います。</summary>
	class ImageSequence {
	private:
		char* filename_;				//ファイル名格納

		sf::Texture* images_;			//シーケンス画像を保持
		unsigned int all_image_frame_;	//全部のシーケンス画像枚数
		unsigned int now_frame_;		//再生中、もしくは一時停止中の現在のフレーム番号
		unsigned int fps_;				//動画fps


		bool is_repeat_;				//繰り返すか
		unsigned int in_frame_;			//リピート用、先頭フレーム
		unsigned int out_frame_;		//リピート用、最終フレーム

		float x_, y_;						//表示する座標
		sf::Clock started_time_;		//スタート時間(ms)
		unsigned int started_frame_;
		bool is_play_;

		float exrate_;					//拡大縮小倍率
		
		bool is_loaded_;				//読み込み完了したか
		bool is_allocated_;				//メモリの確保など
		ImageSequenceResult load_result_;

		std::mutex mtx_;

		void LoadThread();
		void LoadDivThread(const int x_div, const int y_div, const int width, const int height);

	public:
		ImageSequence();
		~ImageSequence();
		
		// *** 読み込み、再生部分 ***

		/// <summary>シーケンス画像のファイルパスを登録します。
		/// この関数を呼び出した時点ではロード処理は発生しません</summary>
		/// <param name='filename'>シーケンス画像を独自の　形式でまとめたファイルのパス付き名前</param>
		jubeat_online::ImageSequenceResult SetSequenceFilename(const char* filename);

		/// <summary>シーケンス画像のロード完了を取得します。</summary>
		/// <returns>読み込み待ちファイル数。0で完了、負の値でロードに失敗</returns>
		int WaitLoadComplete(void);

		/// <summary>シーケンス画像を読み込みます。</summary>
		/// <param name='filename'>シーケンス画像を独自の形式でまとめたファイルのパス付き名前。SetSequenceFilenameを呼び出している場合はNULL指定します。
		/// ここでもファイル名を与えた場合は、こちらの情報を優先します</param>
		/// <returns>0:成功 -1:ロード失敗 -2:ファイル形式不正 -3:ファイル名指定なし</returns>
		/// <remarks>指定された形式の詳細はImageSequence.txtを参照ください</remarks>
		ImageSequenceResult LoadSequence(const char* filename = NULL);

		/// <summary>シーケンス画像を整列配置画像から読み込みます。</summary>
		/// <param name='all_framecount'>取り込む画像の総枚数</param>
		/// <param name='x_div'>X軸方向の分割数</param>
		/// <param name='y_div'>Y軸方向の分割数</param>
		/// <param name='width'>1コマの幅</param>
		/// <param name='height'>1コマの高さ</param>
		/// <param name='filename'>シーケンス画像を独自の形式でまとめたファイルのパス付き名前。SetSequenceFilenameを呼び出している場合はNULL指定します。
		/// ここでもファイル名を与えた場合は、こちらの情報を優先します</param>
		/// <returns>ImageSequenceResult型</returns>
		/// <remarks>指定された形式の詳細はImageSequence.txtを参照ください</remarks>
		ImageSequenceResult LoadDivGraph(const int all_framecount, const int x_div, const int y_div, const int width, const int height, const char *filename = NULL);


		/// <summary>シーケンス画像を動画として再生します。この関数を呼ぶ前にx,y座標を指定しておく必要があります</summary>
		/// <param name='frame'>再生を開始するフレーム番号です。デフォルトは0フレームからです</param>
		/// <returns>int型。成功した場合は0、再生に失敗した場合は-1を返します</returns>
		ImageSequenceResult PlaySequence(const unsigned int frame = 0);

		/// <summary>シーケンス画像を描写します。再生時に描写できます。</summary>
		/// <param name='x'>表示するx座標</param>
		/// <param name='y'>表示するy座標</param>
		/// <param name='ex'>拡大率</param>
		/// <param name='screen_buffer'>スクリーン描写用バッファ</param>
		/// <returns>int型。成功した場合はフレーム番号、再生に失敗した場合は-1を返します</returns>
		int DrawSequence(const float x, const float y, const float ex, sf::RenderTexture* screen_buffer);

		/// <summary>シーケンス画像を描写します。再生時に描写できます。</summary>
		/// <param name='ex'>拡大率</param>
		/// <param name='screen_buffer'>スクリーン描写用バッファ</param>
		/// <returns>int型。成功した場合はフレーム番号、再生に失敗した場合は-1を返します</returns>
		int DrawSequence(const float x, const float y, sf::RenderTexture* screen_buffer);

		/// <summary>シーケンス画像を描写します。再生時に描写できます。</summary>
		/// <returns>int型。成功した場合はフレーム番号、再生に失敗した場合は-1を返します</returns>
		int DrawSequence(sf::RenderTexture* screen_buffer);
		
		/// <summary>シーケンス画像をフレーム静止画として描写します。</summary>
		/// <param name='frame'>表示したいフレーム</param>
		/// <param name='screen_buffer'>スクリーン描写バッファ</param>
		/// <returns>int型。成功した場合は0、失敗した場合は-1を返します</returns>
		int DrawFrame(const float x, const float y, const float ex, const unsigned int frame, sf::RenderTexture* screen_buffer);


		/// <summary>シーケンスの削除を行います。</summary>
		void DeleteSequence(void);
		

		// *** メンバ変数操作関数 ***

		/// <summary>X座標の設定を行います</summary>
		/// <param name='x'>新しく設定するX座標です</param>
		void set_x(const float x);

		/// <summary>X座標の取得を行います</summary>
		/// <returns>int型X座標を返します</returns>
		float x(void) const;


		/// <summary>Y座標の設定を行います</summary>
		/// <param name='x'>新しく設定するY座標です</param>
		void set_y(const float y);

		/// <summary>Y座標の取得を行います</summary>
		/// <returns>int型Y座標を返します</returns>
		float y(void) const;


		/// <summary>ループ時のインポイントの設定を行います
		/// リピートフラグが立っており、アウトポイントに達した場合、このフレームからリピートされます</summary>
		/// <param name='frame'>インポイントとして設定するフレーム番号です</param>
		void set_in_frame(const unsigned int frame);

		/// <summary>ループ時のインポイントの取得を行います</summary>
		/// <returns>unsigned int型でインポイントフレーム番号を返します</returns>
		unsigned int in_frame(void) const;


		/// <summary>ループ時のアウトポイントの設定を行います
		/// リピートフラグが立っており、アウトポイントに達した場合、インポイントフレームへリピートされます</summary>
		/// <param name='frame'>アウトポイントとして設定するフレーム番号です</param>
		void set_out_frame(const unsigned int frame);

		/// <summary>ループ時のアウトポイントの取得を行います</summary>
		/// <returns>unsigned int型でアウトポイントフレーム番号を返します</returns>
		unsigned int out_frame(void) const;
		
		/// <summary>ループ時のインポイントの初期化を行います。これを実行するとインポイントは先頭のフレームに設定されます</summary>
		void InitInPoint(void);

		/// <summary>ループ時のアウトポイントの初期化を行います。これを実行するとアウトポイントは最後尾のフレームに設定されます</summary>
		void InitOutPoint(void);
		

		/// <summary>リピートするかのフラグを設定します</summary>
		/// <param name='flag'>リピートする場合はtrue、しない場合はfalseを指定</param>
		void set_is_repeat(const bool flag);

		/// <summary>リピートするかのフラグを取得します</summary>
		/// <returns>リピート設定の場合true、リピートしない場合falseが返されます</returns>
		bool is_repeat(void) const;

		/// <summary>fpsをセットします</summary>
		/// <param name='fps'>設定するfps</param>
		void set_fps(const unsigned int fps);


	};



}


#endif