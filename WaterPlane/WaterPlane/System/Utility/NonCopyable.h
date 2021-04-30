#pragma once


//派生クラスをコピー禁止にするために
//代入演算子のオーバーロードをprivate化
//	→Singletonの基底クラスだった。
class NonCopyable
{
protected://継承しても引き続き使用可能

	NonCopyable() {};
	virtual ~NonCopyable() {};

private:
	//代入処理に当たるものをprivateにして使用禁止化
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator=(const NonCopyable&);
};