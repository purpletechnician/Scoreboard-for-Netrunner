# Scoreboard-for-Netrunner 0.94
0,94 Fixing bugs in GUI, Up/next-screen-presetation and update-functionality.<br/>
0,93 Added the possibility to show "up-next"-screen between rounds<br/>
0,92 Fixing bugs<br/>
<br/>
Requires SSL-libraries installed (necessary dll:s are shipped in binary zip-file but whole package can be donloaded at https://sourceforge.net/projects/openssl/ or https://sourceforge.net/projects/gnuwin32/files/openssl/0.9.8h-1/openssl-0.9.8h-1-setup.exe)<br/>
<br/>
The software has the following features:<br/>
* Show name of players<br/>
* Countdown timer output based on swiss/top cut or finals round in a tournament<br/>
* Show actual agendapoint for runner/corp. Manual and output to file can be disabled when software is unmanned.<br/>
* Show identity (text and picture) of players runner or corp. Output to file can be disabled when software is unmanned.<br/>
* UpNext-screen can be shown between round with information which round is next and when it starts.<br/>
* Mp3-music can be a added/played.<br/>
<br/>
Below files are created by the software (Output-folder):<br/>
* "Player1_Name.txt"/"Player2_Name.txt": Input: textfield. Shows each player name.<br/>
* "Player1_Score.txt"/"Player2_Score.txt": Shows each player score. Input: Up/down-buttons for each player. Output can be disabled in software.<br/>
* "Player1_Id.txt"/"Player2_Id.txt": Shows each player identity.Input: Choosable field. Output can be disabled in software.<br/>
* "Player1_Ïd.png"/"Player2_Id.png": Hold each player identity icon. Based on above input. Output can be disabled in software.<br/>
* "Clock.txt": Show actual timer value. Input: start/stop timer and radio-button swiss/top cut/grand final.<br/>
* "Round.txt": Shows which round in tourament. Input: textfield.<br/>
* "Round_info.txt": Show "Swiss"/"Top cut"/"Grand final" and round number.<br/>
* "UpNext.png": Shows a "UpNext"-screen used between round. The below "Next_round.txt" and "Next_round_starts.txt" are enabled at the same time. Output can be disabled in software.<br/>
* "Next_round.txt": Shows which next round is on  the "Up next screen". Input: textfield. Output can be enabled/disabled in software.<br/>
* "Next_round_start.txt": Shows when next round starts in the "Up next screen". Input: textfield. Output can be enabled/disabled in software.<br/>
"Xml.xml": Shows Player names, score and time. Output can be enabled/disabled in software. Feature not fully implemented.<br/>
