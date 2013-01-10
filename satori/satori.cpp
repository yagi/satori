#include "Satori.h"

const char* gSatoriName = "Satori";
const char* gSatoriNameW = "���X";
const char* gSatoriCraftman = "Yagi Kushigahama";
const char* gSatoriCraftmanW = "�����l�€";
const char* gSatoriVersion = "phase 123";
const char* gShioriVersion = "3.0";
const char* gSaoriVersion = "1.0";



#ifdef SATORI_DLL
	// Satori�̗B��̎���
	Satori gSatori;
	SakuraDLLHost* SakuraDLLHost::m_dll = &gSatori;
#else
	SakuraDLLHost* SakuraDLLHost::m_dll = NULL;
#endif // SATORI_DLL


// �G�X�P�[�v������
const char escaper::sm_escape_sjis_code[3]={(char)0x9e,(char)0xff,0x00};

// ������������󂯎��A�����o�Ɋi�[���A�u�G�X�P�[�v���ꂽ������v��Ԃ��B
string escaper::insert(const string& i_str)
{
	m_id2str.push_back(i_str);
	//m_str2id[i_str] = m_id2str.size()-1;
	return string() + sm_escape_sjis_code + itos(m_id2str.size()-1) + " ";
}

// �Ώە����񒆂Ɋ܂܂��u�G�X�P�[�v���ꂽ������v�����ɖ߂��B
void escaper::unescape(string& io_str)
{
	const int	max = m_id2str.size();
	for (int i=0 ; i<max ; ++i)
		replace(io_str, string(sm_escape_sjis_code)+itos(i)+" ", m_id2str[i]);
}

// �����o���N���A
void escaper::clear()
{
	//m_str2id.clear();
	m_id2str.clear();
}


// ����]�����A���ʂ�^�U�l�Ƃ��ĉ��߂���
bool Satori::evalcate_to_bool(const Condition& i_cond)
{
	string r;
	if ( !calculate(i_cond.c_str(), r) )
	{
		// �v�Z���s
		return false;
	}
	return  ( r!="0" && r!="�O" );
}
