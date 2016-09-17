/*
����
���@skl_nasm.h�ɑ΂��āh�h�L�������g�̃t�H�[�}�b�g�h�����s������Ƀ��r���h�����
�@�@���s����Bskl_nasm.h�Ɂh�h�L�������g�̃t�H�[�}�b�g�h�����s���Ă͂����Ȃ��B
�@�@�@�@error MSB6006: "cmd.exe" �̓R�[�h 1 �𔺂��ďI�����܂����B


���@getbit.cpp�Ɂh�h�L�������g�̃t�H�[�}�b�g�h�����s����ƃ}�N����`���̐����^�u�����B���A
�@�@�t�@�C���T�C�Y���킸���ɑ�����B


*/

#include "global.h"


//�p�C�v���͂̏�����
//parse_cli()��������������Ɏ��s���邱�ƁB
int Initialize_pf()
{
  if (Mode_PipeInput &&
    Initialize_stdin() == 1)
  {
    if (fdHeadFile != -1)
      _close(fdHeadFile);
    remove(HeadFilePath.c_str());
    return 1;                            //�v���Z�X�I��
  }

  IsClosed_stdin = false;
  GetExtraData_fromStdin = false;
  timeFlushD2VFile = time(NULL);
  tickReadSize_speedlimit = 0;
  tickBeginTime_speedlimit = system_clock::now();
  SpeedLimit = SpeedLimit_CmdLine * 1024 * 1024;
  return 0;
}



//HeadFile�쐬
int Initialize_stdin()
{
  // -i "filepath" -pipe �������Ɏw�肳��Ă����B
  if (NumLoadedFiles != 1)
    return 1;

  //HeadFile
  //  �W�����͂̐擪�����t�@�C���ɏ����o���B
  //  �t�@�C���ɂ��邱�Ƃ�seek�ɑΉ�
  _setmode(_fileno(stdin), _O_BINARY);
  fdStdin = _fileno(stdin);

  //Windows��Temp�t�H���_��DGI_temp_0000000_2���쐬
  {
    DWORD pid = GetCurrentProcessId();
    std::random_device rnd;
    std::mt19937 mt(rnd());
    UINT32 rnd_num = mt() % (1000 * 1000);
    std::string tmp_path;
    tmp_path = "DGI_temp_" + std::to_string(pid) + std::to_string(rnd_num) + "_";
    tmp_path = std::string(_tempnam(NULL, tmp_path.c_str()));
    HeadFilePath = tmp_path;
  }
  fdHeadFile = _open(HeadFilePath.c_str(), _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
  if (fdHeadFile == -1)
    return 1;
  int head_size;
  {
    const double mini_MB = 6.0;
    double size = HeadFileSize_CmdLine;
    size = mini_MB < size ? size : mini_MB;
    head_size = (int)(size * 1024 * 1024);
  }


  //�W�����͂���f�[�^�擾�AHeadFile����
  const int buff_size = 1024 * 32;
  std::vector<BYTE> buff(buff_size);
  int cur_size = 0;                  //HeadFile�ɏ����σT�C�Y

  while (cur_size < head_size)
  {
    int read = _read(fdStdin, &buff.front(), buff_size);
    if (read == -1)
    {
      Sleep(30);
      continue;//fail to connect. wait.
    }
    else if (read == 0)
      break;//end of stream.

    int written = _write(fdHeadFile, &buff.front(), read);
    if (written != read)
      return 1;
    cur_size += written;
  }
  if (head_size <= cur_size)
    head_size = cur_size;
  else
    return 1;//fail to read. too short stream.


  //Infile�ɃZ�b�g
  strcpy(Infilename[0], HeadFilePath.c_str());
  Infile[0] = fdHeadFile;
  _lseeki64(Infile[0], 0, SEEK_SET);

  return 0;
}



