#pragma once
class TextObject;
class ImageObject;

class Money2D :
	public GameObject
{
public:
	Money2D(const char * name);
	~Money2D();
	void Awake()override;
	void Start()override;
	void Update()override;

	//テキストオブジェクトと追加するお金の量を設定。
	void Initialize(int money = 0);

private:
	//入手したお金の量を表示するテキスト。
	TextObject* _MoneyText;

	//入手しました画像。
	ImageObject* _GetImage;

	//ログ用背景。
	ImageObject* _LogBackImage;

	//テキストを表示する場所。
	Vector3 _TextPos = Vector3::zero;

	//画像を表示する場所。
	Vector3 _ImagePos = Vector3::zero;

	//ログ用背景の場所。
	Vector3 _LogBackPos = Vector3::zero;

	//過去に追加されたお金。
	string _AddMoenyLog[5];

	//今ログが何個使われているか。
	int _UseLogCounter = 0;

	//最大表示時間。
	float _LifeTime = 0.0f;

	//現在の時間。
	float _NowTime = 0.0f;

	//α値。
	float _alpha = 0.0f;
};