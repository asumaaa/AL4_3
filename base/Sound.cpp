#include "Sound.h"

SoundData Sound::SoundLoadWave(const char* filename) {

	HRESULT result;

	//-----ファイルオープン------

	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリーモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//-----.wavデータ読み込み-----

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイル化RIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	//ファイル化WAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダ―の確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data ", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	//------読み込んだ音声データをreturn------

	//returnするための音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

//音声データ解放
void Sound::SoundUnload(SoundData* soundData) {

	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};

}

//音声再生
void Sound::SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {

	xAudio2->StartEngine();

	HRESULT result;

	//波形フォーマットをもとにSoundVoiceの生成
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert((SUCCEEDED(result)));

	//再生する波形データの設定
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void Sound::SoundLoopPlayWave(IXAudio2* xAudio2, const SoundData& soundData)
{

	HRESULT result;

	//波形フォーマットをもとにSoundVoiceの生成
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert((SUCCEEDED(result)));

	//再生する波形データの設定
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//ループ
	buf.LoopCount = XAUDIO2_LOOP_INFINITE;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

}

void Sound::SoundStopWave(IXAudio2* xAudio2, const SoundData& soundData)
{

	HRESULT result;
	result = pSourceVoice->Stop();

}

void Sound::SoundSetVolume(float volume)
{
	HRESULT result;
	result = pSourceVoice->SetVolume(volume);

}