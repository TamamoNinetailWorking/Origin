#pragma once


//�h���N���X���R�s�[�֎~�ɂ��邽�߂�
//������Z�q�̃I�[�o�[���[�h��private��
//	��Singleton�̊��N���X�������B
class NonCopyable
{
protected://�p�����Ă����������g�p�\

	NonCopyable() {};
	virtual ~NonCopyable() {};

private:
	//��������ɓ�������̂�private�ɂ��Ďg�p�֎~��
	NonCopyable(const NonCopyable&);
	const NonCopyable& operator=(const NonCopyable&);
};