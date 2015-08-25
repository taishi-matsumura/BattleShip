/**
*@file	Connect.cpp
*@author	亀田
*@brief	通信のテストクラス
*/

#include "Connect.h"


//	初期化
bool Connect::Init()
{
	
	int result = 0;
	result = WSAStartup( MAKEWORD(2,0), &m_wsaData );	///< Winsockの初期化

	if( result != 0 )
	{
		switch(result)
		{
		case WSASYSNOTREADY:///<	ネットワークサブシステムがネットワークへの接続を準備できていない
			DebugMsgBox("WSASYSNOTREADY\n");
			break;

		case WSAVERNOTSUPPORTED:///<	要求されたwinsockのバージョンがサポートされていない
			DebugMsgBox("WSAVERNOTSUPPORTED\n");
			break;

		case WSAEINPROGRESS:///<	ブロッキング操作の実行中であるか、またはサービスプロバイダがコールバック関数を処理している
			DebugMsgBox("WSAEINPROGRESS\n");
			break;

		case WSAEPROCLIM:	///<	winsockが処理できる最大プロセス数に達した
			DebugMsgBox("WSAEPROCLIM\n");
			break;

		case WSAEFAULT:		///<	第二引数であるlpWSAData は有効なポインタではない
			DebugMsgBox("WSAEFAULT\n");
			break;
		}
		return false;
	}
	
	MakeSocket();	///<	ソケット作成

	SettingSocket();	///<	ソケット設定

	return true;
}

//	ソケット生成
bool Connect::MakeSocket()
{
	//	ソケットの生成
	m_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_sock == INVALID_SOCKET ) 
	{
		DebugMsgBox("socket : %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

//	受信メソッド
bool Connect::Receive( char* _buf )
{
	int n = 0;
	memset(_buf, 0, sizeof(_buf));
	//	サーバからのデータ受信
	n = recv( *GetSocket(), _buf, sizeof(_buf), 0 );
	if( n < 0 )
	{
		DebugMsgBox("%d, %s\n", n, _buf);
		return false;
	}
	printf_s(_buf);
		
	return true;
}

//	送信メソッド
bool Connect::Send( SOCKET* _sock, char *_buf )
{

	int n = send(*_sock, _buf, (int)strlen(_buf), 0);
	if( n < 0 )
	{
		DebugMsgBox("send：%d\n", WSAGetLastError() );	///<	送信失敗
		return false;
	}

	return true;
}


//	通信終了
void Connect::EndConnect()
{
	WSACleanup();
}