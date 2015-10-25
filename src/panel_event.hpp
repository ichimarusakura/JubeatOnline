haina#pragma once
#ifndef JUBEAT_ONLINE_PANEL_EVENT_HEADER
#define JUBEAT_ONLINE_PANEL_EVENT_HEADER


//***************TEMPORARY***************//
//#include "msf.h"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <mutex>

namespace jubeat_online {
	enum SequenceType : int16_t {
		Offset = 0x0000,
		Bpm = 0x0001,
		Note = 0x0002,
		Hold = 0x0003,
		Release = 0x0004
	};

	typedef struct {
		SequenceType type;
		short int panel_number;
		int time;
	}Sequence;



	class Scene : public sf::RenderTexture{
	public:
		Scene() {}
		virtual void Init() {}
		virtual int Update() { return 0; }
		~Scene() {}
	protected:
	private:
	};



	class Position {
	public:
		int x, y;
	};

	class Panel {
	public:
		Position pos;
	};

	class Me {
	public:
		Panel a[16];
		Panel getPos(int n) { return a[n]; }
	};
}



namespace jubeat_online {

	/// <summary>1件分のパネルの押下履歴が格納されているクラス</summary>
	class PanelEventQueue {
	public:
		/// <summary>Sequence型のデータ内容。</summary>
		Sequence data_;

	private:
		//リンクリスト次の要素のアドレス
		PanelEventQueue * next_element_;
	};

	/// <summary>パネル入力の設定、チェックをするSceneクラス継承</summary>
	class PanelEventConsole : public Scene {
	public:
		/// <summary>パネル入力の設定及びチェックをするためのコンソール画面を展開します。</summary>
		/// <param name='window'>画面バッファ</param>
		void Update(sf::RenderTexture window);
		
	};

	/// <summary>パネルの入力を取得するためのクラス</summary>
	class PanelEvent {
	public:

		/// <summary>初期化を行う関数。コンストラクタとはまた別。この時別スレッドを作成する。</summary>
		/// <param name='ini_fpath'>初期化するためのiniファイルパス。</param>
		/// <returns>0:成功 -1:異常発生</returns>
		static int Init(const std::string ini_fpath);

		/// <summary>セッティングコンソールとして存在するSceneクラス継承なクラスを取得。</summary>
		/// <returns>コンソール画面を展開するSceneクラスを継承したPanelEventConsoleを取得</returns>
		static PanelEventConsole console(void);

		/// <summary>1件のイベントをアンキューする。</summary>
		/// <returns>パネルイベント履歴の要素を返す。</returns>
		static PanelEventQueue getEvent(void);

		/// <summary>キュー要素数を返す</summary>
		/// <returns>キュー要素数</returns>
		static unsigned int get_queue_num(void);

		/// <summary>キュー履歴を記録するかどうかのフラグを設定する。falseにした場合、すでにあったキューは全て破棄される。</summary>
		/// <param name='flag'>trueなら記録する、falseなら履歴を初期化し記録しない</param>
		static void is_queue(const bool flag);

		/// <summary>押下中のパネル枠を表示する。関数が呼ばれたタイミングで押されているものの枠に限る。</summary>
		/// <param name='window'>画面バッファ</param>
		/// <returns>0:成功 -1:異常発生</returns>
		static int DrawFrame(sf::RenderTexture window);

		/// <summary>オーバーフローしているか確認する。この関数を呼んだ場合、フラグはリセットされる。</summary>
		/// <returns>オーバーフローフラグ</returns>
		static bool is_overflow(void);

	private:
		static PanelEventQueue * que_head;
		static PanelEventQueue * que_tail;

		static unsigned int que_num_;

		static PanelEventConsole console_;

		static std::mutex mtx_;
	};

}


#endif