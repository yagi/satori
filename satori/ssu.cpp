//------------------------------------------------
//
//	���X�������[�e�B���e�B���C�u�����@ssu.dll
//
#include	"SaoriHost.h"
#include	<map>

class ssu : public SaoriHost {
public:
	virtual SRV		request(deque<string>& iArguments, deque<string>& oValues);
};
SakuraDLLHost* SakuraDLLHost::m_dll = new ssu;


static SRV	call_ssu(string iCommand, deque<string>& iArguments, deque<string>& oValues)
{
	// ���O�Ɩ��߂��֘A�t����map
	typedef SRV (*Command)(deque<string>&, deque<string>&);
	static map<string, Command>	theMap;
	if ( theMap.empty() )
	{ 
		// ���񏀔�
		#define	d(iName)	\
			SRV	_##iName(deque<string>&, deque<string>&); \
			theMap[ #iName ] = _##iName
		// ���߈ꗗ�̐錾�Ɗ֘A�t���B
		d(calc);			d(calc_float);		d(if);				d(unless);
		d(nswitch);			d(switch);			d(iflist);			d(substr);
		d(split);			d(join);			d(replace);			d(replace_first);	d(erase);
		d(erase_first);		d(count);			d(compare);			d(compare_head);
		d(compare_tail);	d(length);			d(is_empty);		d(is_digit);
		d(is_alpha);		d(zen2han);			d(han2zen);			d(hira2kata);
		d(kata2hira);		d(sprintf);			d(reverse);			d(at);
		d(choice);
		#undef	d
	}

	// ���߂̑��݂��m�F
	map<string, Command>::iterator i = theMap.find(iCommand);
	if ( i==theMap.end() )
		return SRV(400, string()+"Error: '"+iCommand+"'�Ƃ������O�̖��߂͒�`����Ă��܂���B");

	// ���ۂɌĂ�
	return	i->second(iArguments, oValues);
}

SRV	ssu::request(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	SRV(400, "���߂��w�肳��Ă��܂���");

	// �ŏ��̈����͖��ߖ��Ƃ��Ĉ���
	string	theCommand = iArguments.front();
	iArguments.pop_front();
	return	call_ssu(theCommand, iArguments, oValues);
}


// �����������

#ifdef POSIX
#  include      "../_/Utilities.h"
#else
#  include	<windows.h>
#  include	<mbctype.h>
#endif
#include	"../_/stltool.h"

/* �u�\�v��2�o�C�g�ڂ́u\�v�ł���̂ŁA�G�X�P�[�v����K�v������B */
static const char	zen[] = 
	"�@�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y����������������������������������������������������"
	"�O�P�Q�R�S�T�U�V�W�X�I�h���������f�i�j���`�b�e�o�{���p�����H�Q�[�O�����u�G�F�v�A�B�E���~�|�C�D�m�n"
	"�A�C�E�G�I�J�L�N�P�R�T�V�X�Z\x83\x5c�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~�����������������������������@�B�D�F�H�������J�K�A�B";
static const char	han[] = 
	" ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
	"0123456789!\"#$%&'()=~|`{+*}<>?_-^\\@[;:],.�/*-,.[]"
	"�������������������������������������������ܦݧ��������ߤ�";
static const char	kata[] = "�A�C�E�G�I�J�L�N�P�R�T�V�X�Z\x83\x5c�^�`�c�e�g�i�j�k�l�m�n�q�t�w�z�}�~���������������������������������@�B�D�F�H�������b�K�M�O�Q�S�U�W�Y�[�]�_�a�d�f�h�o�r�u�x�{�p�s�v�y�|";
static const char	hira[] = "�����������������������������������ĂƂȂɂʂ˂̂͂Ђӂւق܂݂ނ߂���������������񂟂�������������K�M�O�Q�S���������������Âłǂ΂тԂׂڂς҂Ղ؂�";
static const char	zen_alpha[] = "�`�a�b�c�d�e�f�g�h�i�j�k�l�m�n�o�p�q�r�s�t�u�v�w�x�y����������������������������������������������������";
static const char	han_alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const char	zen_digit[] = "�O�P�Q�R�S�T�U�V�W�X";
static const char	han_digit[] = "0123456789";

extern	bool calc(string& ioString);
extern	bool calc_float(string& ioString);

#include	<sstream>

// ���p/�S�p�𓯓��Ɉ�������ŕ�������Ԃ�
int	sjis_strlen(const char* p) {
	int	n=0;
	for (int i=0 ; p[i] != '\0' ; i += _ismbblead(p[i]) ? 2 : 1 )
		++n;
	return	n;
}

// ���p/�S�p�𓯓��Ɉ��������n�����ړ��A���ߎ���NULL
const char*	sjis_at(const char* p, int n) {
	for (int i=0 ; i<n ; ++i) {
		if ( *p == '\0' )
			return	NULL;
		p += _ismbblead(*p) ? 2 : 1;
	}
	return	p;
}

bool	printf_format(const char*& p, deque<string>& iArguments, stringstream& os) {
	assert(*p=='%');
	if ( iArguments.empty() )
		return	false;	// �u�������Ώۂ�����

	++p;
	string	str = iArguments.front();
	iArguments.pop_front();

	// �t���O�w��ǂݍ���
	bool	isLeft=false, isZeroFill=false, isSharp=false;
	enum { MINUS_ONLY, MINUS_AND_PLUS, IF_PLUS_THEN_PUT_SPACE } SignMode = MINUS_ONLY;
	while (true) {
		if ( *p == '-' ) { isLeft=true; ++p; }
		else if ( *p == '+' ) { SignMode=MINUS_AND_PLUS; ++p; }
		else if ( *p == '0' ) { os.fill('0'); os<<internal; isZeroFill=true; ++p; }
		else if ( *p == ' ' ) { os.fill(' '); os<<internal; SignMode=IF_PLUS_THEN_PUT_SPACE; ++p; }
		else if ( *p == '#' ) { isLeft=true; ++p; }
		else break;
	}

	// ���w��ǂݍ���
	int	width=0;
	bool	isReadWidth = false;
	if ( *p=='*' ) {
		isReadWidth = true;
		++p;
	} else {
		while ( *p>='0' && *p<='9' ) {
			width = width*10 + (*p - '0');
			++p;
			os.width(width);
		}
	}

	// ���x�w��ǂݍ���
	/*int	precision=0;
	if ( *p == '.' ) {
		++p;
		while ( *p>='0' && *p<='9' ) {
			precision = precision*10 + (*p - '0');
			++p;
		}
		os.precision(precision);
	}*/

	// �T�C�Y�w��q�͖��Ή�

	// �ϊ������ɉ����đ}��
	int	n = atoi(str.c_str());
	switch (*p) {
	case 's': os << str; break;
	case 'c': os << (char)n; break;
	case 'C': os << (unsigned short)n; break;
	case 'd': os << n; break;
	case 'i': os << oct << n; break; 
	case 'o': os << oct << n; break;
	case 'u': os << (unsigned int)n; break;
	case 'x': case 'X': break;
	case 'f': break;
	case 'e': case 'E': break;
	case 'p': break;
	default: return false;
	}
	++p;
	return	true;
}

string	sprintf(deque<string>& iArguments) {
	stringstream s;
	string	str = iArguments.front();
	iArguments.pop_front();
	const char* p = str.c_str();
	while ( *p!='\0' ) {
		if ( *p=='%' && printf_format(p, iArguments, s) )
			continue;
		if ( _ismbblead(*p) ) {
			s << *p++; s << *p++;
		} else {
			s << *p++;
		}
	}
	return	s.str();
}


SRV _calc(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc(exp) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B"); // �u�\�v��2�o�C�g�ڂ́u\�v
	return	exp;
}

SRV _calc_float(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc_float(exp) )
	    return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");
	return	exp;
}

SRV _if(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 || iArguments.size()>3 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc(exp) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");
	if ( exp!="0" )
		return	iArguments[1];	// �^
	else
		if ( iArguments.size()==3 )
			return	iArguments[2];	// �U
		else
			return	SRV(204);	// �U��else�Ȃ�
}

SRV _unless(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 || iArguments.size()>3 )
		return	SRV(400, "�����̌�������������܂���B");
	string	exp = iArguments[0];
	if ( !calc(exp) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");
	if ( exp=="0" )
		return	iArguments[1];	// �U
	else
		if ( iArguments.size()==3 )
			return	iArguments[2];	// �^
		else
			return	SRV(204);	// �^��else�Ȃ�
}

SRV _nswitch(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 )
		return	SRV(400, "����������܂���B");
	if ( !calc(iArguments[0]) )
		return	SRV(400, string()+"'"+iArguments[0]+"' �����v�Z�s\x94\x5c�ł��B");

	int	n = stoi(iArguments[0]);
	//iArguments.pop_front();
	//if ( iArguments.size()>n )
	if ( n>0 && iArguments.size()>n )
		return	SRV(200, iArguments[n]);
	else
		return	SRV(204);
}

SRV _switch(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 )
		return	SRV(400, "����������܂���B");

	const string	lhs = iArguments[0];
	const int max = iArguments.size();
	for (int i=1 ; i<max ; i+=2) {
		if ( i==max-1 ) // ��������̏ꍇ�A�Ō�̂P��else��
			return	SRV(200, iArguments[i]);
		string	exp = string("(") + lhs + ")==(" + iArguments[i] + ")";
		if ( !calc(exp) )
			return	SRV(400, string()+"switch��"+itos((i-1)/2+1)+"�ځA�� '"+exp+"' �͌v�Z�s\x94\x5c�ł����B");
		if ( exp!="0" )
			return	SRV(200, iArguments[i+1]);
	}
	return	SRV(204);
}

SRV _iflist(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<2 )
		return	SRV(400, "����������܂���B");

	const string	lhs = iArguments[0];
	const int max = iArguments.size();
	for (int i=1 ; i<max ; i+=2) {
		if ( i==max-1 ) // ��������̏ꍇ�A�Ō�̂P��else�����B�����܂ł����疳�����ł����Ԃ��B
			return	SRV(200, iArguments[i]);
		string	exp = lhs + iArguments[i];
		if ( !calc(exp) )
			return	SRV(400, string()+"iflist��"+itos((i-1)/2+1)+"�ځA�� '"+exp+"' �͌v�Z�s\x94\x5c�ł����B");
		if ( exp!="0" )
			return	SRV(200, iArguments[i+1]);
	}
	return	SRV(204);
}


SRV _substr(deque<string>& iArguments, deque<string>& oValues) {

	if ( iArguments.size()<1 )
		return	SRV(400, "����������܂���B");

	// �Ώە�����
	const char* p = iArguments[0].c_str();
	if ( iArguments.size()==1 )
		return	SRV(200, p); // �����P�Ȃ�S�̂�Ԃ�

	const int	len = sjis_strlen(p);

	// �n�_
	int	start = atoi(iArguments[1].c_str());
	if ( start < 0 )
		start = len + start;

	// �n�_����̃I�t�Z�b�g�l
	int offset = (iArguments.size()<=2) ? len : atoi(iArguments[2].c_str());
	if ( offset==0 || offset==INT_MIN ) // INT_MIN�̎��͕������]�������Ȃ��̂�0�����B
		return	SRV(204);
	if ( offset<0 ) {
		start += offset;
		offset = -offset;
	}
	assert(offset >= 0 );

	if ( start < 0 )
		start = 0;
	if ( start >= len )
		return	SRV(204);
	if ( start + offset >= len )
		offset = len - start;

	const char* const start_p = sjis_at(p, start);
	const char* const end_p = sjis_at(start_p, offset);
	return	SRV(200, string(start_p, end_p));
}

SRV _split(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 || iArguments.size()>3 )
		return	SRV(400, "�����̌�������������܂���B");

	strvec	vec;
	if ( iArguments.size()==1 ) {
		split(iArguments[0],vec);
	}
	else if ( iArguments.size()==2 ) {
		split(iArguments[0],iArguments[1],vec);
	}
	else {
		if ( !calc(iArguments[2]) )
			return	SRV(400, "split�̑�R�����͎��܂��͐��l�ł���K�v������܂��B");
		split(iArguments[0],iArguments[1],vec,stoi(iArguments[2]));
	}

	for ( strvec::iterator i=vec.begin() ; i!=vec.end() ; ++i )
		oValues.push_back(*i);
	return	SRV(200, itos(vec.size()));
}

SRV _join(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	SRV(400, "�����̌�������������܂���B");

	string	r = iArguments[1];
	for (int n=2 ; n<iArguments.size() ; ++n)
		r += iArguments[0] + iArguments[n];
	return	r;
}

SRV _replace(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=3 )
		return	SRV(400, "�����̌�������������܂���B");
	replace(iArguments[0], iArguments[1], iArguments[2]);
	return	SRV(200, iArguments[0]);
}

SRV _replace_first(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=3 )
		return	SRV(400, "�����̌�������������܂���B");
	replace_first(iArguments[0], iArguments[1], iArguments[2]);
	return	iArguments[0];
}

SRV _erase(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	erase(iArguments[0], iArguments[1]);
	return	iArguments[0];
}

SRV _erase_first(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	erase_first(iArguments[0], iArguments[1]);
	return	iArguments[0];
}

SRV _count(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	itos( count(iArguments[0], iArguments[1]) );
}

SRV _compare(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	(strcmp(iArguments[0].c_str(), iArguments[1].c_str())==0) ? "1" : "0";
}

SRV _compare_head(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	compare_head(iArguments[0], iArguments[1]) ? "1" : "0";
}

SRV _compare_tail(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=2 )
		return	SRV(400, "�����̌�������������܂���B");
	return	compare_tail(iArguments[0], iArguments[1]) ? "1" : "0";
}

SRV _length(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	"0";
	return	itos( sjis_strlen(iArguments[0].c_str()) );
}

SRV _is_empty(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 )
		return	"1";
	if ( iArguments[0].empty() )
		return	"1";
	else
		return	"0";
}

SRV _is_digit(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 || iArguments[0].empty() )
		return	"0";
	int	i;
	for ( const char* p = iArguments[0].c_str() ; *p ; p += (_ismbblead(*p)?2:1) ) {
		for ( i=0 ; i<20 ; i+=2)
			if ( p[0]==zen_digit[i] && p[1]==zen_digit[i+1] )
				break;
		if ( i<20 )
			continue;
		for ( i=0 ; i<10 ; ++i)
			if ( p[0]==han_digit[i] )
				break;
		if ( i<10 )
			continue;
		return	"0";
	}
	return	"1";
}

SRV _is_alpha(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()<1 || iArguments[0].empty() )
		return	"0";
	return	arealphabets(iArguments[0]) ? "1" : "0";
}

SRV _zen2han(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");

	char	before[3]="�@", after[2]=" ";
	string&	str=iArguments[0];
	for (int n=0 ; n<sizeof(han) ; ++n) {
		before[0]=zen[n*2];
		before[1]=zen[n*2+1];
		after[0]=han[n];
		replace(str, before, after);
	}
	return	str;
}

SRV _han2zen(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");

	char	before[2]=" ", after[3]="  ";
	string&	str=iArguments[0];
	for (int n=0 ; n<sizeof(han) ; ++n) {
		before[0]=han[n];
		after[0]=zen[n*2];
		after[1]=zen[n*2+1];
		replace(str, before, after);
	}
	return	str;
}

SRV _hira2kata(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");

	string&	str=iArguments[0];
	for (int i=0 ; str[i]!='\0' ; i+=_ismbblead(str[i])?2:1) {
		for (int j=0 ; j<sizeof(hira) ; j+=2) {
			if ( str[i]==hira[j] && str[i+1]==hira[j+1] ) {
				str[i]=kata[j];
				str[i+1]=kata[j+1];
			}
		}
	}
	return	iArguments[0];
}

SRV _kata2hira(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.size()!=1 )
		return	SRV(400, "�����̌�������������܂���B");

	string&	str=iArguments[0];
	for (int i=0 ; str[i]!='\0' ; i+=_ismbblead(str[i])?2:1) {
		for (int j=0 ; j<sizeof(hira) ; j+=2) {
			if ( str[i]==kata[j] && str[i+1]==kata[j+1] ) {
				str[i]=hira[j];
				str[i+1]=hira[j+1];
			}
		}
	}
	return	iArguments[0];
}

SRV _sprintf(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.empty() )
		return	SRV(400, "����������܂���B");
	return	sprintf(iArguments);
}

SRV _reverse(deque<string>& iArguments, deque<string>& oValues) {
	if ( iArguments.empty() )
		return	SRV(400, "����������܂���B");

	string	r;
	const char* p = iArguments[0].c_str();
	while (*p != '\0') {
		const int len = _ismbblead(*p)?2:1;
		r = string(p, len) + r;
		p += len;
	}

	return	r;
}

SRV _at(deque<string>& iArguments, deque<string>& oValues) {

	if ( iArguments.size()==2 ) {
		const char* p = sjis_at(iArguments.at(0).c_str(), stoi(iArguments.at(1)));
		return	(p==NULL || *p=='\0') ? "" : string(p, _ismbblead(*p)?2:1);
	}
	//else if ( iArguments.size()==3 ) {
	//}
	else
		return	SRV(400, "����������������܂���B");
}



/*
if ( compare_head(theCommand, "tm") ) {
	string	TimeCommands(const string& iCommand, const deque<string>& iArguments);
	return	TimeCommands(theCommand, iArguments);
}
*/
SRV _choice(deque<string>& iArguments, deque<string>& oValues)
{
	if ( iArguments.size()==0 )
	{
		return	"";
	}
	return iArguments[ rand() % iArguments.size() ];
}