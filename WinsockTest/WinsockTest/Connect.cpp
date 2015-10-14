/**
*@file	Connect.cpp
*@author	亀田
*@brief	通信のテストクラス
*/

#include "Connect.h"


//	初期化
bool Connect::Init( bool _bSockType )
{
	m_sockType = _bSockType;
	
	m_ownSock = NULL;
	m_partnersSock = NULL;
	
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
	m_ownSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_sock == INVALID_SOCKET ) 
	{
		DebugMsgBox("socket : %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

bool Client::SettingSocket( int _ports, char* _domainStr )
{
	if( m_sockType ){
		// 接続先指定用構造体の準備
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(_ports);	///<	ポート番号
		m_addr.sin_addr.S_un.S_addr = inet_addr( _domainStr );
	}
	else
	{
		// ソケットの設定
		//	アドレスとポート番号を決めてソケットに情報の紐付けを行う
		m_addr.sin_family = AF_INET;
		m_addr.sin_port = htons(_ports);
		m_addr.sin_addr.S_un.S_addr = INADDR_ANY;
		//	bind関数でソケットをIPアドレスとポート番号と組み合わせをして、
		//	インターフェースの協力関係を作る。
		bind( m_ownSock, (struct sockaddr *)&addr, sizeof(addr));
		
	}
	return true;
}


//	受信メソッド
bool Connect::Connection()
{
	if( m_sockType ){
		// サーバーに接続
		if (connect(m_ownSock, (struct sockaddr *)&m_addr, sizeof(m_addr)))
		{
			puts("connect に失敗しました");
			getch();
			return -1;
		}
		
	}
	else{
		// TCPクライアントからの接続要求を待てる状態にする
		listen( ownSock, 5);
		int len = sizeof( m_client );
		//	要求があったらaccept関数で要求を受け取ったという合図をおくり
		//	Client側のソケットデータをもらう
		m_partnersSock = accept( m_onwSock, (struct sockaddr *)&m_client, &len);
		if (m_partnersSock == INVALID_SOCKET)
		{   printf("accept : %d\n", WSAGetLastError());
		    
		}
	}
	
	//int n = 0;
	//memset(_buf, 0, sizeof(_buf));
	////	サーバからのデータ受信
	//n = recv( *GetSocket(), _buf, sizeof(_buf), 0 );
	//if( n == SOCKET_ERROR )
	//{
	//	printf_s("%d, %s\n", n, _buf);
	//	return false;
	//}
	//else
	//{
	//	printf_s(_buf);
	//}
	//memset(_buf, 0, sizeof(_buf));
    //
	//int nRtn=1;
	//char*pt=_buf;
	////タイムアウトを約5秒にするためのループ
	//while(nRtn && SOCKET_ERROR!=nRtn && 0<(bfSize-(_buf-pt))){
	//	for(int i=0;i<50;i++){
	//		nRtn = recv( *GetSocket(), _buf, bfSize-(_buf-pt), 0 );//受信
	//		if(0<=nRtn){
	//			printf_s("recvError：%d\n", WSAGetLastError() );
	//			break;
	//		}
	//		else
	//		{
	//			printf_s("通信成功%s\n",_buf);
	//			Send( &m_sock, m_buf);
	//			Sleep(100);
	//		}
	//	}
	//	_buf+=nRtn;
	//}
	//*_buf='\0';
	//
	//return (_buf-pt)? true: false;

	//return true;
}

//	受信メソッド
bool Connect::Receive( char *_buf, int _bufSize )
{
	//	念のためバッファーの初期化
	memset(_buf,0,_bufSize);
	
	recv(sockw, (char*)testBuf, bufSize, 0 );
	if (n < 0)
	{   puts("recv に失敗しました");
	    getch();
	    return false;
	}
	return true;
}

//	送信メソッド
bool Connect::Send( char *_buf, int _bufSize )
{
	n= send(sockw, (const char*)testObj, bufSize, 0);
	if (n < 0)
	{   puts("send に失敗しました");
	    getch();
	    return false;
	}
	return true;
}



//	通信終了
void Connect::EndConnect()
{
	if( m_partnersSock != NULL )
		closesocket(m_partnersSock);
	
	closesocket(m_ownSock);
	WSACleanup();
}