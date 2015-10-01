//***************************************************
//*   ImageSequence Class Library					*
//*													*
//*		Programmed by ほわいと						*
//***************************************************


#pragma once
#ifndef JUBEAT_ONLINE_IMAGESEQUENCE_H_
#define JUBEAT_ONLINE_IMAGESEQUENCE_H_

#ifndef NULL
#define NULL 0
#endif

namespace jubeatOnline {

	//c_ImageSequence関数について
	//	シーケンス画像をもとにアニメーションを展開します。
	//	この関数で、シーケンス画像のロードから描画まですべて行います
	//	ただし、シーケンス画像を動画として再生するプログラムですので
	//	座標や表示状態などは外部から指定する必要があります。

	/// <summary>シーケンス画像をもとにアニメーションを展開します。
	/// このクラスで、シーケンスの画像のロードから描画まですべて行います。</summary>
	class c_ImageSequence {
	private:
		char* filename_;				//ファイル名格納

		int* images_;					//シーケンス画像を保持
		unsigned int all_image_frame_;	//全部のシーケンス画像枚数
		unsigned int now_frame_;		//再生中、もしくは一時停止中の現在のフレーム番号
		unsigned int fps_;				//動画fps

		bool is_repeat_;				//繰り返すか
		unsigned int in_frame_;			//リピート用、先頭フレーム
		unsigned int out_frame_;		//リピート用、最終フレーム

		int x_, y_;						//表示する座標
		unsigned int started_time_;		//スタート時間(ms)

		bool is_expand;					//拡大縮小表示するか
		double exrate;					//拡大縮小倍率
		
	public:
		
		// *** 読み込み、再生部分 ***

		/// <summary>シーケンス画像のファイルパスを登録します。
		/// この関数を呼び出した時点ではロード処理は発生しません</summary>
		/// <param name='filename'>シーケンス画像を独自の　形式でまとめたファイルのパス付き名前</param>
		void SetSequenceFilename(const char* filename);

		/// <summary>シーケンス画像を読み込みます。</summary>
		/// <param name='filename'>シーケンス画像を独自の形式でまとめたファイルのパス付き名前。SetSequenceFilenameを呼び出している場合はNULL指定します。
		/// ここでもファイル名を与えた場合は、こちらの情報を優先します</param>
		/// <returns>成功:0 ロードに失敗:-1 ファイル形式不正:-2 ファイル名指定なし:-3</returns>
		/// <remarks>指定された形式の詳細はImageSequence.txtを参照ください</remarks>
		int LoadSequence(const char* filename = NULL);

		/// <summary>シーケンス画像を整列配置画像から読み込みます。</summary>
		/// <param name='all_framecount'>取り込む画像の総枚数</param>
		/// <param name='x_div'>X軸方向の分割数</param>
		/// <param name='y_div'>Y軸方向の分割数</param>
		/// <param name='width'>1コマの幅</param>
		/// <param name='height'>1コマの高さ</param>
		/// <param name='filename'>シーケンス画像を独自の形式でまとめたファイルのパス付き名前。SetSequenceFilenameを呼び出している場合はNULL指定します。
		/// ここでもファイル名を与えた場合は、こちらの情報を優先します</param>
		/// <returns>成功:0 ロードに失敗:-1 ファイル形式不正:-2 ファイル名指定なし:-3</returns>
		/// <remarks>指定された形式の詳細はImageSequence.txtを参照ください</remarks>
		int LoadDivGraph(const int all_framecount, const int x_div, const int y_div, const int width, const int height, const char *filename = NULL);

		/// <summary>シーケンス画像を動画として再生します</summary>
		/// <param name='x'>描写するx座標をここで指定します</param>
		/// <param name='y'>描写するy座標をここで指定します</param>
		/// <param name='frame'>再生を開始するフレーム番号です。デフォルトは0フレームからです</param>
		/// <returns>int型。成功した場合は0、再生に失敗した場合は-1を返します</returns>
		int PlaySequence(const int x, const int y, const unsigned int frame = 0);

		/// <summary>シーケンス画像を動画として再生します。この関数を呼ぶ前にx,y座標を指定しておく必要があります</summary>
		/// <param name='frame'>再生を開始するフレーム番号です。デフォルトは0フレームからです</param>
		/// <returns>int型。成功した場合は0、再生に失敗した場合は-1を返します</returns>
		int PlaySequence(const unsigned int frame = 0);
		

		// *** メンバ変数操作関数 ***

		/// <summary>X座標の設定を行います</summary>
		/// <param name='x'>新しく設定するX座標です</param>
		void X(const int x);

		/// <summary>X座標の取得を行います</summary>
		/// <returns>int型X座標を返します</returns>
		int X(void) const;


		/// <summary>Y座標の設定を行います</summary>
		/// <param name='x'>新しく設定するY座標です</param>
		void Y(const int y);

		/// <summary>Y座標の取得を行います</summary>
		/// <returns>int型Y座標を返します</returns>
		int Y(void);


		/// <summary>ループ時のインポイントの設定を行います
		/// リピートフラグが立っており、アウトポイントに達した場合、このフレームからリピートされます</summary>
		/// <param name='frame'>インポイントとして設定するフレーム番号です</param>
		void InPoint(const unsigned int frame);

		/// <summary>ループ時のインポイントの取得を行います</summary>
		/// <returns>unsigned int型でインポイントフレーム番号を返します</returns>
		unsigned int InPoint(void) const;


		/// <summary>ループ時のアウトポイントの設定を行います
		/// リピートフラグが立っており、アウトポイントに達した場合、インポイントフレームへリピートされます</summary>
		/// <param name='frame'>アウトポイントとして設定するフレーム番号です</param>
		void OutPoint(const unsigned int frame);

		/// <summary>ループ時のアウトポイントの取得を行います</summary>
		/// <returns>unsigned int型でアウトポイントフレーム番号を返します</returns>
		unsigned int OutPoint(void) const;
		
		/// <summary>ループ時のインポイントの初期化を行います。これを実行するとインポイントは先頭のフレームに設定されます</summary>
		void InitInPoint(void);

		/// <summary>ループ時のアウトポイントの初期化を行います。これを実行するとアウトポイントは最後尾のフレームに設定されます</summary>
		void InitOutPoint(void);
		

		/// <summary>リピートするかのフラグを設定します</summary>
		/// <param name='flag'>リピートする場合はtrue、しない場合はfalseを指定</param>
		void RepeatFlag(const bool flag);

		/// <summary>リピートするかのフラグを取得します</summary>
		/// <returns>リピート設定の場合true、リピートしない場合falseが返されます</returns>
		bool RepeatFlag(void) const;

	};


	typedef c_ImageSequence		ImageSequence;
	typedef c_ImageSequence*	lpImageSequence;

}


#endif