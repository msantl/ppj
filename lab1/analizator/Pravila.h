vector< string > dohvatiStanja() {
	vector< string > ret;
	ret.push_back("S_a");
	return ret;
}
vector< string > dohvatiImena() {
	vector< string > ret;
	ret.push_back("P");
	ret.push_back("Q");
	return ret;
}
vector< pair< string, string > > dohvatiPravila() {
	vector< pair< string, string > > ret;
	ret.push_back(make_pair("S_a", "aaa"));
	ret.push_back(make_pair("S_a", "aa*"));
	ret.push_back(make_pair("S_a", "\\n"));
	return ret;
}
map< int, vector< pair< string, string > > > dohvatiAkcije() {
	map< int, vector< pair< string, string > > >ret;
	ret[0].push_back(make_pair("P", ""));
	ret[1].push_back(make_pair("Q", ""));
	ret[1].push_back(make_pair("VRATI_SE", "1"));
	ret[2].push_back(make_pair("-", ""));
	ret[2].push_back(make_pair("NOVI_REDAK", ""));
	return ret;
}
