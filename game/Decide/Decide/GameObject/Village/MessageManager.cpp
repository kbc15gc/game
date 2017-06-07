#include"stdafx.h"
#include "MessageManager.h"

namespace
{
	//メンバ変数の情報設定
	Support::DATARECORD Messagedata[] =
	{
		{ "MessageID",Support::DataTypeE::INT , offsetof(struct MessageInfo,MessageID),sizeof(int) },
		{ "Title",Support::DataTypeE::STRING, offsetof(struct MessageInfo,Title),sizeof(char) * 256 },
		{ "Text",Support::DataTypeE::STRING, offsetof(struct MessageInfo,Text),sizeof(char) * 256 },
		{ "NextID",Support::DataTypeE::INT , offsetof(struct MessageInfo,NextID),sizeof(int) },
		{ "EventID",Support::DataTypeE::INT , offsetof(struct MessageInfo,EventID),sizeof(int) },
	};
}

MessageManager*  MessageManager::_Instance;

MessageInfo* MessageManager::GetMess(const int & id)
{
	MessageInfo* mess;
	try
	{
		mess = _Messages.at(id);
	}
	catch (std::out_of_range& out)
	{
		//範囲外アクセス
		mess = nullptr;
	}
	
	return mess;
}

void MessageManager::_LoadCSV()
{
	//ファイルパス
	const char* filepath = "Asset/Data/TextData/Message.csv";
	//ファイルから歴史の情報読み込み
	Support::LoadCSVData<MessageInfo>(filepath, Messagedata, ARRAY_SIZE(Messagedata), _Messages);
}
