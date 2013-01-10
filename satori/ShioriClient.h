#include "SakuraDLLClient.h"

// SHIORI/3.0�N���C�A���g
class ShioriClient : public SakuraDLLClient
{
public:
	ShioriClient() {}
	virtual ~ShioriClient() {}

	bool load(
		const string& i_sender,
		const string& i_charset,
		const string& i_base_folder,
		const string& i_fullpath);

	int ShioriClient::request(
		const string& i_id, // OnBoot�Ƃ�
		const vector<string>& i_references, // Reference?
		bool i_is_secure, // SecurityLevel
		string& o_value, // �x���Ԃ���������X�N���v�g�╶���񃊃\�[�X
		vector<string>& o_references // �����߂�l�B�ʏ��OnCommunicate�ł����g��Ȃ�
		);
};