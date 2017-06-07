#pragma once

//namespace {
	//���b�Z�[�W�̏��
	struct MessageInfo :Noncopyable
	{
		int MessageID;		//���b�Z�[�W��ID
		char Title[256];	//�^�C�g��
		char Text[256];		//�{��
		int NextID;			//���̃��b�Z�[�W��ID
		int EventID;		//�C�x���g��ID
	};
//}

//���b�Z�[�W���Ǘ�����N���X
class MessageManager
{
public:
	//�Y�����ɑΉ��������b�Z�[�W�擾
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
	//CSV����e�L�X�g�̏��ǂݍ���
	void _LoadCSV();
private:
	//���b�Z�[�W����
	vector<MessageInfo*> _Messages;
	static MessageManager* _Instance;
};