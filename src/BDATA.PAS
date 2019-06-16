Program BuildData;

Uses
  Crt;

Const
  FileName = 'time.tls';
  TextFile = 'Offsets.txt';
  Files    = 15;
  LF = CHR(13) + CHR(10);
  Sources : Array(.1..Files.) Of String[12] = (
  'backg.bin',
  'backgr1.gif',
  'backgr2.gif',
  'bump.gif',
  'cmap.gif',
  'final.xm',
  'flare.gif',
  'map.gif',
  'map2.gif',
  'map3.gif',
  'meta.bin',
  'names.gif',
  'snakes.bin',
  'time.gif',
  'tls.bin'


  );

Type
  BufferType = Array[1..1] Of Byte;

Var
  F, FSource : File;
  TextF : Text;
  I : Integer;
  Buffer : ^BufferType;
  Result : Word;
  HeaderS : String;

Begin
  HeaderS := 'ÖÄÄ-ùú'       + LF +
             'º -- TLS --  '   + LF +
             '    "time"  º' + LF +
             '       úù-ÄÄ½' + CHR(26);

  GetMem(Buffer, 64000);

  Assign(F, FileName);
  Rewrite(F, 1);
  Assign(TextF, TextFile);
  Rewrite(TextF);
  BlockWrite(F, HeaderS[1], 64);

  For I := 1 To Files Do
  Begin
    Assign(FSource, Sources[I]);
    Reset(FSource, 1);
    WriteLn(TextF, Sources[I]:12,'  : ',FilePos(F));
    BlockRead(FSource, Buffer^, 64000, Result);
    Repeat
      WriteLn('Adding ',Result,' bytes from ',Sources[I]);
      BlockWrite(F, Buffer^, Result);
      BlockRead(FSource, Buffer^, 64000, Result);
    Until Result = 0;
    Close(FSource);
  End;

  Close(TextF);
  Close(F);
  FreeMem(Buffer, 64000);
End.