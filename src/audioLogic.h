#pragma once

#include <SFML/Audio.hpp>


/**
 * music クラス
 * 音楽関連のオブジェクト
 */
class MusicObject {
private:

	// 楽曲ファイルを読みに行くためのパスです
	char *filePath;
	char *fileNames[];

public:

	/**
	 * filePathのセッターメソッド
	 * @param filePath
	 */
	void setFilePath(char *filePath) {
		this->filePath = filePath;
	}

	/**
	 * filePathのゲッターメソッド
	 * @return
	 */
	char *getFilePath() {
		return this->filePath;
	}

	/**
	 * fileNamesのセッターメソッド
	 * @param fileName
	 */
	void setFileNames(char* fileName, int index) {
		this->fileNames[index] = fileName;
	}

	/**
	 * fileNamesのゲッターメソッド
	 * @return
	 */
	char* getFileNames(int index) {
		return this->fileNames[index];
	}

};

/**
 * MusicObjectを参照し楽曲の操作を行います
 *
 */
class MusicPlayerObject {
private:
	sf::Music music;
	MusicObject musicObject_;

public:
	/**
	 * MusicObjectクラスから楽曲の再生を行います
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