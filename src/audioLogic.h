#pragma once

#include <SFML/Audio.hpp>


/**
 * music �N���X
 * ���y�֘A�̃I�u�W�F�N�g
 */
class MusicObject {
private:

	// �y�ȃt�@�C����ǂ݂ɍs�����߂̃p�X�ł�
	char *filePath;
	char *fileNames[];

public:

	/**
	 * filePath�̃Z�b�^�[���\�b�h
	 * @param filePath
	 */
	void setFilePath(char *filePath) {
		this->filePath = filePath;
	}

	/**
	 * filePath�̃Q�b�^�[���\�b�h
	 * @return
	 */
	char *getFilePath() {
		return this->filePath;
	}

	/**
	 * fileNames�̃Z�b�^�[���\�b�h
	 * @param fileName
	 */
	void setFileNames(char* fileName, int index) {
		this->fileNames[index] = fileName;
	}

	/**
	 * fileNames�̃Q�b�^�[���\�b�h
	 * @return
	 */
	char* getFileNames(int index) {
		return this->fileNames[index];
	}

};

/**
 * MusicObject���Q�Ƃ��y�Ȃ̑�����s���܂�
 *
 */
class MusicPlayerObject {
private:
	sf::Music music;
	MusicObject musicObject_;

public:
	/**
	 * MusicObject�N���X����y�Ȃ̍Đ����s���܂�
	 *
	 */
	/*int play(MusicObject musicObject, int musicIndex) {
		char *fileName;

		strcpy(fileName, musicObject.getFilePath());
		strcpy(fileName, musicObject.getFileNames(musicIndex));

		if (!music.openFromFile(fileName)) {
			return -1;
		}

		return 0;
	}*/


};