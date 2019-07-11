# Scoreboard-for-Netrunner 1.02<br/>
1,03 Bugfixing SSL-libraries<br/>
1,02 Adding Magnum Opus, System Core 2019, Downfall. Bugfixing and adaptation to newest QT-environment<br/>
1,01 Bugfixing<br/>
1,00 Bugfixing<br/>
0,99 Bugfixing<br/>
0,98 UI-improvements and bugfixing<br/>
0,97 Added possibility to search for cards and "show" them to the left or right<br/>
0,96 Fixing bugs in GUI and code<br/>
0,95 Fixing bugs in GUI and code<br/>
0,94 Fixing bugs in GUI, Up/next-screen-presentation and update-functionality<br/>
0,93 Added the possibility to show "up-next"-screen between rounds<br/>
0,92 Fixing bugs<br/>
<br/>
Requires SSL-libraries installed (necessary dll:s for 32/64bit are shipped in binary zip-file but whole package can be downloaded at https://slproweb.com/products/Win32OpenSSL.html)<br/>
<br/>
The software has the following features:<br/>
• Output the below information to separate text- or image-files<br/>
• Show name of players<br/>
• Countdown timer output based on swiss/top cut or finals round in a tournament<br/>
• Show actual agendapoint for runner/corp. Manual and can be disabled when software is unmanned.<br/>
• Show Corp and Runner identity (text and picture). Can be disabled when software is unmanned.<br/>
• UpNext-screen can be shown between round with information which round is next and when it starts.<br/>
• Search-function of Cards and possibility to show them on left or right.<br/>
• Mp3-music can be a added/played.<br/>
<br/>
Below files are created by the software (Output-folder):<br/>
• "Player1_Name.txt"/"Player2_Name.txt": Shows each player name. Input: Textfield.<br/>
• "Player1_Score.txt"/"Player2_Score.txt": Shows each player score. Input: Up/down-buttons for each player. Output can be disabled in software.<br/>
• "Player1_Id.txt"/"Player2_Id.txt": Shows each player identity.Input: Choosable field. Output can be disabled in software.<br/>
• "Player1_Id.png"/"Player2_Id.png": Hold each player identity icon. Based on above input. Output can be disabled in software.<br/>
• "Clock.txt": Show actual timer value. Input: start/stop timer and radio-button swiss/top cut/grand final.<br/>
• "Round.txt": Shows which round in tourament. Input: textfield.<br/>
• "Round_info.txt": Show "Swiss"/"Top cut"/"Grand final" and round number.<br/>
• "UpNext.png": Shows a "UpNext"-screen used between round. The below "Next_round.txt" and "Next_round_starts.txt" are enabled at the same time. Output can be disabled in software.<br/>
• "Next_round.txt": Shows which next round is on  the "Up next screen". Input: textfield. Output can be enabled/disabled in software.<br/>
• "Next_round_start.txt": Shows when next round starts in the "Up next screen". Input: textfield. Output can be enabled/disabled in software.<br/>
• "Card_left.png"/"Card_right.png": Shows the card choosen from a search. Cards can afterwards be hidden in software.
• "Xml.xml": Shows Player names, score and time. Output can be enabled/disabled in software. Feature not fully implemented.<br/>
