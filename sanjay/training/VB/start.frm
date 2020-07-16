VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   1665
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   4305
   LinkTopic       =   "Form1"
   ScaleHeight     =   1665
   ScaleWidth      =   4305
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text2 
      Height          =   495
      Left            =   120
      TabIndex        =   2
      Text            =   "Text2"
      Top             =   1080
      Width           =   4095
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   120
      Top             =   120
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.CommandButton cmdSend 
      Caption         =   "Send"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   2655
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   120
      Width           =   2655
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub CmdConnect_Click()
    If (UCase(CmdConnect.Caption) = "CONNECT") Then
        CmdConnect.Caption = "Disconnect"
        MSComm1.PortOpen = True
    Else
        CmdConnect.Caption = "Connect"
        MSComm1.PortOpen = False
    End If
End Sub

Private Sub cmdSend_Click()
    MSComm1.Output = Text1.Text
End Sub

Private Sub Form_Load()
    Text1.Text = ""
    Text2.Text = ""
    MSComm1.CommPort = 1
    MSComm1.Settings = "9600,N,8,1"
    MSComm1.InputLen = 0
    MSComm1.RThreshold = 20
    MSComm1.PortOpen = 1
End Sub

Private Sub MSComm1_OnComm()

    Dim mData As String
    Select Case MSComm1.CommEvent
        Case comEvReceive   ' Received RThreshold # of characters
            Text2.Text = MSComm1.Input
    End Select
End Sub

