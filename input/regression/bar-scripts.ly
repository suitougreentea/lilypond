#(ly:set-option 'old-relative)
\version "1.9.8"
\header{
texidoc="

Markings that are attached to (invisible) barlines are 
delicate: the are attached to the rest of the score without the score
knowing it.  Consequently, they fall over  often.
"
}


onestaff =  \new Staff\notes\relative c''  {
	\property Staff.instr = instr
	\property Staff.instrument = instrument \mark "B"
	 c1 \mark "A" \break c2  c2 \break
}

grstaff =  \notes \relative c'' \context GrandStaff <<
	\new Staff {

	\property Staff.instr = instr
	
	 \mark "B" \break c1 \mark "A" c2  }
	\new Staff { c1 c2  }
>>


\score {\grstaff}

