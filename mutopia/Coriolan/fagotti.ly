\header{
filename =	 "fagotti.ly";
% %title =	 "Ouvert\\"ure zu Collins Trauerspiel \\"Coriolan\\" Opus 62";
description =	 "";
composer =	 "Ludwig van Beethoven (1770-1827)";
enteredby =	 "JCN";
copyright =	 "public domain";


}

\version "1.2.0";

\include "fagotto-1.ly"
\include "fagotto-2.ly"

$fagotti_staff = \context Staff = fagotti <
	\property Staff.midiInstrument = "bassoon"
	\property Staff.instrument = "2 Fagotti"
	\property Staff.instr = "Fg."
	\clef "bass";
	\notes \context Voice=fagotti < 
		\global
		\$fagotto1
		\$fagotto2
	>
>

