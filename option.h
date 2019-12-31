#pragma once

//#define _DEBUG
		//きんだんのわざ
//#undef _DEBUG
		//きんだんのわざ



//各種設定項目。公開する前にしっかり確認！/////////////////////

	//#define _NO_USE_GGSOUND_			//GGSoundをインクルードしない。（もちろん音は出ない。）

	#define _PLAY_SOUND_

	const int SOUND_VOLUME	= 7000;		//基本値：7000

	const int MONTH_LOCK	= 12;		//基本値：12

	//#define _HARD_LOCK_				//定義されていると、ハードモードがプレイ不能

	//#define _ADD_DOT_BMP_				//画像ファイル読み込みの末尾に.bmpを付加

	//#define _NO_SHOP_					//ショップが使用不能になる。

	//#define _TRIAL_VERSION_			//体験版デモが入る。

#ifdef _TRIAL_VERSION_
class CTrialDemo;
extern	CTrialDemo		TrialDemo;
#endif

//デバッグ用オプション//////////////////////
#ifdef _DEBUG
	//#define _NO_OPTION_
				//全てのオプションを使わない

#ifndef _NO_OPTION_

/*その他のオプション///////////////////*/
	//#define _SET_FPS_	2
				//FPSをn倍

	//#define _SHOW_TUTORIAL
				//イベントスキップ中でもチュートリアルを表示

	//#define _SET_BGM_	1
				//再生するBGMを固定

	//#define _GAME_PAD_OFF_
				//ゲームパッドを使用不能にする

	#define _ALL_SOUND_TEST_
				//サウンドテストで全ての曲が使用できるようになる。

	//#define _SET_FLAG_	
		#define _SET_FLAG_ID_	56
		#define _SET_FLAG_NO_	60
				//フラグを固定。Flag[ID] = NO; 
/*一般データ確認用オプション///////////////////*/

	//#define _SHOW_FLAG_
		#define _SHOW_FLAG_MIN_ 27
		#define _SHOW_FLAG_MAX_ 27
				//フラグを表示。

	//#define _DRAW_DATA_
		#define _DRAW_DATA_FRIEND_	CMapMode	//フレンド指定するべきクラス
		#define _DRAW_DATA_NAME_	MapMode.mBGMWait
		//#define _DRAW_DATA_MIN_		10
		//#define _DRAW_DATA_MAX_		10
					//一般的なデータを出力する
					//配列で無い場合、最小と最大をコメントアウトすれば、自動で処理する。
				

	//#define _CHECK_MAPCHIP_
				//マップチップが読み込まれたか確認する

/*グラフィック系オプション///////////////////*/

	//#define _PLAYER_IMAGE_		73
				//マップ上の主人公の絵

	//#define _BATTLE_IMAGE_		191
				//戦闘中の絵

	//#define _BATTLE_BG_			15
				//戦闘中の背景

	#define _SHOW_OBJECT_IMG_NO_
				//スペースキーで、画像番号を表示。

/*戦闘系オプション///////////////////*/

	//#define _FREE_SKILL_
				//スキル使い放題

	//#define _ALL_SKILL_	
				//全スキル使用可能 

	//#define _ALL_SKILL_NO_ 192
				//規定のスキルから64個を使用可能。_ALL_SKILL_より優先

	#define _SET_STATUS_	22
				//ステータス異常を一つつける。	

	//#define _ENEMY_LOCK_
		#define _ENEMY_LOCK_TYPE_	71
		#define _ENEMY_LOCK_LV_		4
		#define _ENEMY_LOCK_NO_		4
				//敵パーティーを固定。

	//#define	_ENEMY_SKILL_LOCK_		14
				//敵が特定のスキルしか使えなくなる。

	//#define _BATTLE_FPS_	60
				//戦闘中のFPSを変更

	#define _SUPER_MAN_ 10000
				//与えるダメージがn倍、受けるダメージが０

	//#define _POOR_MAN_
				//与えるダメージが０

	//#define _LOCK_HP_	1
				//指定したHPで戦闘開始

	//#define _REFLECT_	
				//強制的に反射

	#define _ESCAPE_	
				//絶対に逃走に成功できる。

	#define _DELETE_ESCAPE_ENEMY_	
				//逃走後に敵が消滅

	//#define _ALL_CRITICAL_	
				//絶対にクリティカルが出る。

	//#define	_GET_EXP_	50000.0
				//獲得経験値n倍

	#define _MONSTER_KNOWLEDGE_
				//生物学自動習得


/*マップ系オプション///////////////////*/

	#define _SHOW_MAP_NO_
				//マップ番号を表示。

	#define _SHOW_PLAYER_POS_
				//主人公の現在座標を表示

	#define _SHOW_ENEMY_
				//出現する敵を表示

	#define _SHOW_OBJECT_NO_
				//マップオブジェクトの番号を表示。

	#define _SHOW_TRESURE_NO_
				//宝箱番号を表示

	#define _SHOW_ENEMY_LV_
				//敵のレベルを表示

	#define _SHOW_OUT_OBJ_
				//範囲外や落下、いしのなかにいるオブジェの番号を表示

	//#define _SUPER_JUMP_			20
				//ジャンプ力×n

	//#define _PLAYER_SPEED_		8
				//マップ上の主人公の速度。変更時のみ。

	#define _HAWK_EYE_
				//鷹の目自動発動

	//#define _HAWK_EYE_EVERYTIME_
				//鷹の目常時発動

	//#define _HIDDEN_PLAYER_
				//プレイヤーを表示しない

	//#define _HIDDEN_MESSAGE_
				//メッセージを表示しない

/*初期化系オプション///////////////////*/
	#define _ALL_ITEM_
				//倉庫をアイテムでいっぱいにする。
				//仲間キャラも全員最初から仲間になる。
				//店もすべて使える様にする。

	//#define _LEVEL_LOCK_ 60
				//初期レベルを固定させる。

	//#define _AUTO_LEARN_
				//誕生石の自動習得。

	//#define _LOCK_FULL_SCREEN_ true
				//スクリーンモードを固定。

/*データ出力系オプション///////////////////*/

	//#define _OUTPUT_EXIT_
				//出力終了後、速やかに終了

	/*攻略情報関連///////////////////*/
	//#define _OUTPUT_ENEMY_
				//敵のパラメータを計算してHTMLに吐きます。
				//debugoutputにオプションあり。

	//#define _OUTPUT_ENEMY_SKILL_
				//敵のスキルを出力します。
				//debugoutputにオプションあり。
	//#define _OUTPUT_SKILL_USED_
				//そのスキル、本当に使われてますか？


	//#define _OUTPUT_GET_TRESURE_
				//宝箱の獲得状況を、ロード時に出力します。

	//#define _OUTPUT_ENEMY_MONTH_
				//すべての敵の、月別出撃状況をHTMLに吐きます。下とオプションを共用。
	//#define _OUTPUT_KEY_DATA_
				//カギの入手、消費個数を出力します。下とオプションを共用。
	//#define _OUTPUT_ALL_ITEM_
				//すべての獲得アイテムと、獲得できないアイテムをHTMLに吐きます。下とオプションを共用。
	//#define _OUTPUT_TRESURE_
				//月別の獲得できる宝箱の中身をHTMLに吐きます。
				//debugoutputにオプションあり。

	//#define _OUTPUT_SKILL_
				//全キャラの習得スキルをHTMLに吐きます。

	//#define _OUTPUT_EXP_
				//レベル差による獲得経験値の倍率を出力します。

	//#define _OUTPUT_GROW_PARAM_
				//各キャラクターの、月別の理論上の最大パラメータを出力します。
		#define _OUTPUT_GROW_PARAM_STEP_ 1


  /*画像情報関連///////////////////*/
	//#define _OUTPUT_ENEMY_IMG_
				//マップ上での敵キャラクタの画像を出力します。

	//#define _OUTPUT_ITEM_IMG_
				//アイテムのアイコンごとの名前と説明文を出力します。

	//#define _OUTPUT_SKILL_EFFECT_
				//エフェクト画像ごとに、使用されている技・魔法を出力します。


  /*Wiki用///////////////////*/
	//#define _OUTPUT_WIKI_ITEM_
				//Wiki用アイテム一覧

	//#define _OUTPUT_WIKI_CSKILL_
				//Wiki用Ｃスキル一覧

	//#define _OUTPUT_WIKI_MONTH_
				//Wiki用月情報一覧

	//#define _OUTPUT_WIKI_PLAYER_
				//Wiki用プレイヤー情報一覧

  /*攻略用///////////////////*/
	//#define _OUTPUT_MAP_BMP_
				//攻略用マップ画像
		#define _OUTPUT_MAP_BMP_SIZE_	16

	//#define _OUTPUT_MONTH_ENEMY_REAL_
				//超詳細に月ごとの敵の出現状況を出力
		//#define _OUTPUT_MONTH_ENEMY_REAL_LD_MODE_	//ハードラストダンジョン専用モード

	#define _OUTPUT_DB_CSKILL_
				//攻略用Cスキル一覧

	#define _OUTPUT_DB_ASKILL_
				//攻略用Aスキル一覧

	//#define _OUTPUT_DB_EQUIP_
				//攻略用装備品一覧

	//#define _OUTPUT_DB_UI_
				//攻略用消耗品一覧

	#define _OUTPUT_DB_PLAYER_
				//攻略用キャラ情報一覧

	#define _OUTPUT_DB_MONTH_
				//攻略用月データ

	#define _OUTPUT_DB_ENEMY_
				//攻略用敵データ

	#define _OUTPUT_DB_RANKING_
				//攻略用能力値ランキング

	#define _OUTPUT_DB_SHOP_
				//攻略用ショップ一覧

	#define _OUTPUT_DB_BS_
				//攻略用誕生石リスト



#endif
#endif

////////////////////////////////////////////////////////////////