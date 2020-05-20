object MainForm: TMainForm
  Left = 192
  Top = 124
  Width = 299
  Height = 472
  Caption = #1042#1077#1089#1077#1083#1099#1081' '#1082#1086#1090#1080#1082
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Memo: TMemo
    Left = 16
    Top = 24
    Width = 145
    Height = 369
    TabOrder = 0
  end
  object StartBtn: TButton
    Left = 176
    Top = 48
    Width = 89
    Height = 49
    Caption = #1057#1090#1072#1088#1090
    TabOrder = 1
    OnClick = StartBtnClick
  end
  object StopBtn: TButton
    Left = 176
    Top = 120
    Width = 89
    Height = 49
    Caption = #1057#1090#1086#1087
    TabOrder = 2
    OnClick = StopBtnClick
  end
  object Timer: TTimer
    Interval = 100
    OnTimer = TimerTimer
    Left = 208
    Top = 208
  end
end
