\version "1.7.6"
\header {

texidoc = "With the new chord syntax it's possible to associate
fingerings uniquely with notes. This makes horizontal fingering much
easier to process."

}

\score {
    \notes \relative c'{
	<<g_0>> 4
	<<g_1>> 4
	<<c''^5>> 4
	<<e,^6>> 4
	<<f,_1>> 4
 	<< c-1 e-3>> 4
	<< c-1  e-3 g-5 >> 4
	<< c-1  e-2 g-3 b-5 >> 4
	\property Voice.fingersHorizontal = ##t
	<< c-1  e-3 g-5 >> 4
	\property Voice.fingersHorizontal = ##t
	<< c-1  e-2 g-3 b-5 >> 4
	
}

}

%% new-chords-done %%
%% new-chords-done %%
%% new-chords-done %%
