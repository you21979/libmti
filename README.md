libmti
======

malloc trace information

2001年頃に作った骨董品
mallocをチェーンでつないでメモリリークをトレースするために作った気がする


機能
====
	メモリーリークの検出
	バッファーオーバーランの検出（mallocでアロケートした領域のみ）
	２重開放バグの検出(コンソールに表示される)
	開放後のメモリアクセスの検出(中身が全てクリアされているのでデータが化けます)

	基本的に落ちたり落ちなかったりするバグが
	落ちる要因のあるところで落ちるようになると思います。

	確保したばかりで初期化していないメモリの中身は適当な値(DEF:0xCC)で初期化しています。
	開放したメモリの中身は適当な値(DEF:0xDD)で初期化しています。
	これらはバグを見つけやすくするためなので0x00で初期化する変更をしないでください。


動作原理
========
	調査対象がソースファイル *.c の場合
	コンパイル時にマクロによって別のデバッグ用関数に置き換える
	malloc		->	mt_malloc
	free		->	mt_free	
	この場合、メモリ管理機能以外にもより詳細な情報を取得できる
	(アロケートされたファイル名、行番号など)

	ライブラリ *.a の場合
	リンク時に同じ関数エントリを持つデバッグ用ライブラリに置き換える
	メモリ管理機能のみ利用できる

コンパイル方法
==============
	コマンドラインから
	$ make
	でコンパイルできます。

組み込み方法
============
	#include "memdbg.h"
	を調べたいソースファイルに組み込む


トレース方法
============
	任意の場所に以下を組み込む
	{
	char buf[1024000];
	DEBUGEHD("メモリリークチェック開始");
	mt_all_print_info_to_buf(buf);
	DEBUGEHD(buf);
	DEBUGEHD("メモリリークチェック終了");
	}

	又は

	{
	char buf[1024000];
	puts("メモリリークチェック開始");
	mt_all_print_info_to_buf(buf);
	puts(buf);
	puts("メモリリークチェック終了");
	}

リンク方法
==========
	memdbg.aをmakefileに追加
	gcc -o sample1 sample1.c memdbg.a



----------------------------------------------------------------

mti.c
=====
trace info regist and access routine

mtil.c
======
interface layer

msl.c
=====
safe layer





