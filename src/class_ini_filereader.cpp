//author white
//INIFileReader class source

#include <vector>

#include "ini_filereader.hpp"
#include "output_logtext.hpp"


jubeat_online::INIFileReader::INIFileReader() {
	//element_num_ = 0;
	data_ = NULL;
	filename_ = "";
}

jubeat_online::INIFileReader::INIFileReader(const std::string filename) {
	INIFileReader();
	LoadFile(filename);
}

int jubeat_online::INIFileReader::LoadFile(const std::string filename) {

	filename_ = filename;

	//�t�@�C���I�[�v��
	file_.open(filename,std::ios::in);
	if (!file_) {	//�ǂݍ��ݎ��s
		OutputLogtext::OutputS("ini�t�@�C��" + filename + "�̓ǂݍ��݂Ɏ��s���܂���","INIFileReader");
		return -1;
	}

	//�s�œǂ�
	std::string line;
	std::string section = "";
	INIFileReaderElements * tmp;
	while (!file_.eof()) {
		//�s�ǂݍ���
		std::getline(file_, line);

		int cur = 0;
		int cur2 = 0;
		const char *cline = line.c_str();

		//�󔒂͔�΂�
		for (cur = 0; cline[cur] == ' '; cur++);
		
		
		switch (cline[cur]) {
		case '\n': continue;	//���̍s��
		case '\0': continue;	//���̍s��
		case ';': continue;		//���̍s��
		case '[':
			//�Z�N�V����
			cur2 = ++cur;
			for (; cline[cur] != ']' && cline[cur] != '\0' && cline[cur] != '\n'; cur++);
			section = line.substr(cur2, cur - cur2);
			break;

		default:
			//key and value
			tmp = new INIFileReaderElements;
			
			//����
			tmp->section_ = section;
			tmp->next = NULL;
			
			
			for (cur2 = cur; cline[cur] != '=' && cline[cur] != '\0'; cur++);
			tmp->key_ = line.substr(cur2, cur - cur2);
			
			
			for (cur2 = cur + 1; cline[cur] != '\0' && cline[cur] != '\n'; cur++);
			if (cur2 >= cur) tmp->value_ = "";
			else tmp->value_ = line.substr(cur2, cur - cur2);

			INIFileReaderElements ** i;
			//�����N���X�g�̍Ō����������
			for (i = &data_; *i != NULL; i = &((*i)->next));

			//�ǉ�
			*i = tmp;

			break;
		}
		
	}

	file_.close();

	return 0;
}

void jubeat_online::INIFileReader::Delete() {
	//�S�Ă̗v�f�̊J��
	Delete(data_);	
}

void jubeat_online::INIFileReader::Delete(INIFileReaderElements * d) {
	//�S�Ă̗v�f�̊J��
	if (d == NULL) return;
	Delete(d->next);
	delete d;
}

int jubeat_online::INIFileReader::ReloadFile() {
	Delete();
	return LoadFile(filename_);
}

std::string jubeat_online::INIFileReader::getValue(const std::string section, const std::string key, const std::string def_str) const {
	
	for (INIFileReaderElements * i = data_; i != NULL; i = i->next) {
		if (i->section_ == section && i->key_ == key) return i->value_;
	}
	
	return def_str;
}

int jubeat_online::INIFileReader::RewriteValue(const std::string section, const std::string key, std::string value) {

	//�t�@�C���I�[�v��
	file_.open(filename_, std::ios::in);
	if (!file_) {	//�ǂݍ��ݎ��s
		OutputLogtext::OutputS("ini�t�@�C��" + filename_ + "�̓ǂݍ��݂Ɏ��s���܂���", "INIFileReader");
		return -1;
	}


	std::string line;
	std::vector<std::string> lines;
	while (!file_.eof()) {
		std::getline(file_, line);
		lines.push_back(line);
	}

	file_.close();


	file_.open(filename_, std::ios::out | std::ios::trunc);
	if (!file_) {	//�ǂݍ��ݎ��s
		OutputLogtext::OutputS("ini�t�@�C��" + filename_ + "�̏������݂Ɏ��s���܂���", "INIFileReader");
		return -1;
	}


	//�s�œǂ�
	std::string sec = "";
	bool match_sec = (section == "" ? true : false);
	bool edited = false;
	std::string output;

	for (int i = 0; i < lines.size();i++) {
		//�s�ǂݍ���
		line = lines[i];
		output = line;
		int cur = 0;
		int cur2 = 0;
		const char *cline = line.c_str();

		//�󔒂͔�΂�
		for (cur = 0; cline[cur] == ' '; cur++);


		switch (cline[cur]) {
		case '\n': break;	//���̍s��
		case '\0': break;	//���̍s��
		case ';': break;		//���̍s��
		case '[':
			//�Z�N�V����
			cur2 = ++cur;
			for (; cline[cur] != ']' && cline[cur] != '\0' && cline[cur] != '\n'; cur++);
			sec = line.substr(cur2, cur - cur2);
			if (match_sec == false) {
				if (sec == section) match_sec = true;
			}
			else {
				//�}�b�`������̂��Ȃ������ꍇ
				output = key + "=" + value + "\n" + output;
				edited = true;
				match_sec = false;
			}
			break;

		default:
			if (!match_sec) break;
			//key and value
			INIFileReaderElements tmp;

			//����
			tmp.section_ = section;
			tmp.next = NULL;


			for (cur2 = cur; cline[cur] != '=' && cline[cur] != '\0'; cur++);
			tmp.key_ = line.substr(cur2, cur - cur2);

			if (tmp.section_ == section && tmp.key_ == key) {
				//�������ҏW����|�C���g
				output = key + "=" + value;
				match_sec = false;
				edited = true;
			}


			break;
		}

		if (i < lines.size() - 1) output += "\n";
		file_.write(output.c_str(), output.length());

	}

	if (edited == false) {
		if(!match_sec) output = "\n\n[" + section + "]\n" + key + "=" + value;	//�����ɒǉ�
		else output = key + "=" + value;
		file_.write(output.c_str(),output.length());
	}

	file_.close();

	return 0;
}