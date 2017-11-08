/**
* JsonData���Ǘ�����N���X�̒�`.
*/
#pragma once


/**
* JsonData���Ǘ�����N���X.
* �V���O���g���N���X.
*/
class JsonData : Noncopyable
{
public:

	/**
	* �R���X�g���N�^
	*/
	JsonData()
	{
	}

	/**
	* �f�X�g���N�^.
	*/
	~JsonData()
	{
	}

	/**
	* �ǂݍ���.
	* Asset\Data\SaveData�t�H���_����json�t�@�C��.
	*
	* @param filename	�t�@�C����.
	*
	* @return true or false.
	*/
	bool Load(string filename);

	/**
	* ��������.
	*
	* @param filename	�t�@�C����.
	*/
	void Save(string filename);

	/**
	* �f�[�^�I�u�W�F�N�g�̎擾.
	*
	* @return �I�u�W�F�N�g.
	*/
	picojson::object& GetDataObject()
	{
		return DataObject_;
	}

	/**
	* �f�[�^�I�u�W�F�N�g�̎擾.
	*
	* @param key	�L�[.
	*
	* @return �I�u�W�F�N�g.
	*/
	picojson::object& GetDataObject(string key)
	{
		if (DataObject_[key].is<picojson::object>())
		{
			return DataObject_[key].get<picojson::object>();
		}
		FB_ASSERT(false, "�f�[�^�����݂��Ă��܂���");
	}

	/**
	* �f�[�^�I�u�W�F�N�g�̎擾.
	*
	* @param key	�L�[.
	*
	* @return �I�u�W�F�N�g.
	*/
	picojson::array& GetDataArray(string key)
	{
		if (DataObject_[key].is<picojson::array>())
		{
			return DataObject_[key].get<picojson::array>();
		}
		FB_ASSERT(false, "�f�[�^�����݂��Ă��܂���");
	}

	/**
	* �I�u�W�F�N�g��o�^����.
	*
	* @param key	�L�[.
	* @param obj	�I�u�W�F�N�g.
	*/
	void SetDataObject(string key, picojson::object& obj)
	{
		DataObject_[key] = picojson::value(obj);
	}

	/**
	* �I�u�W�F�N�g��o�^����.
	*
	* @param key	�L�[.
	* @param arr	�z��.
	*/
	void SetDataObject(string key, picojson::array& arr)
	{
		DataObject_[key] = picojson::value(arr);
	}

private:

	/** �f�[�^�I�u�W�F�N�g. */
	picojson::object DataObject_;

};

