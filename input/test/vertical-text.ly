\score{
	<
		\context Staff = o \notes\relative c,,, { 
			\stemDown [c8 c c c] c4 c4 \break [c8 c c c] c4 c4
		}
		\context Lyrics \lyrics{
			BLA gag _2 < { BLA gag } { BLA gag la } > _2 
		}
		\context Staff = t \notes\relative c''''' { 
			\stemUp [c8 c c c] c4 c [c8 c c c] c4 c
		} 
	>
	\paper{

	}
}

\version "1.3.96"; 
