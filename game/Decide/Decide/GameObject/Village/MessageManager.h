#pragma once

	//メッセージの情報
struct MessageInfo :Noncopyable
{
	int MessageID;		//メッセージのID
	char Title[256];	//タイトル
	char Text[256];		//本文
	int NextID;			//次のメッセージのID
	int EventID;		//イベントの種類を識別するID。
	int EventNo;		//実行するイベントの番号。
	char VoicePath[256];//ボイスのパス
};

//メッセージを管理するクラス
class MessageManager
{
public:
	//添え字に対応したメッセージ取得
	MessageInfo* GetMess(const int& id);

	static MessageManager* Instance()
	{
		if (_Instance == nullptr)
		{
			_Instance = new MessageManager();
			_Instance->_LoadCSV();
		}
		return _Instance;
	}
private:
	//CSVからテキストの情報読み込み
	void _LoadCSV();
private:
	//メッセージたち
	vector<unique_ptr<MessageInfo>> _Messages;
	static MessageManager* _Instance;
};