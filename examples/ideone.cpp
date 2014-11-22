    #include <string>
    #include <algorithm>
    #include <iostream>
    #include <fstream>
    
    int main()
    {
        std::ios_base::sync_with_stdio(false);
        std::string moves;
        for(int i = 32 ; i < 126; ++i) moves.push_back(char(i));
        std::string out;
        std::vector<std::string> lines;
        sort(begin(moves), end(moves));
        int movelen = moves.length();
        int maxit = movelen + 1;
        std::ofstream of("Generated.txt");
        if(of.is_open()) {
        for(int a0 = 0; a0 < movelen; a0++)
        for(int a1 = 0; a1 < movelen; a1++)
        for(int a2 = 0; a2 < movelen; a2++)
        for(int a3 = 0; a3 < movelen; a3++)    
        for(int a4 = 0; a4 < movelen; a4++)
        for(int a5 = 0; a5 < movelen; a5++)        
        for(int a6 = 0; a6 < movelen; a6++)        
        for(int a7 = 0; a7 < movelen; a7++)
        for(int a8 = 0; a8 < movelen; a8++)
        for(int a9 = 0; a9 < movelen; a9++)
        for(int aa = 0; aa < movelen; aa++)
        for(int ab = 0; ab < movelen; ab++)
        for(int ac = 0; ac < movelen; ac++)
        for(int ad = 0; ad < movelen; ad++)
        for(int ae = 0; ae < movelen; ae++)
        for(int af = 0; af < movelen; af++)
        for(int ag = 0; ag < movelen; ag++)
        for(int ah = 0; ah < movelen; ah++)
        for(int ai = 0; ai < movelen; ai++)
        for(int aj = 0; aj < movelen; aj++)
        for(int ak = 0; ak < movelen; ak++)
        for(int al = 0; al < movelen; al++)
        for(int am = 0; am < movelen; am++)             
        for(int an = 0; an < movelen; an++)
        for(int ao = 0; ao < movelen; ao++)
        for(int ap = 0; ap < movelen; ap++)
        for(int aq = 0; aq < movelen; aq++)
        for(int ar = 0; ar < movelen; ar++)
        for(int as = 0; as < movelen; as++)
        for(int at = 0; at < movelen; at++)
        for(int au = 0; au < movelen; au++)
        for(int av = 0; av < movelen; av++)
        for(int aw = 0; aw < movelen; aw++)
        for(int ax = 0; ax < movelen; ax++)
        for(int ay = 0; ay < movelen; ay++)
        for(int az = 0; az < movelen; az++)
        for(int b1 = 0; b1 < movelen; b1++)
        for(int b2 = 0; b2 < movelen; b2++)
        for(int b3 = 0; b3 < movelen; b3++)
        for(int b4 = 0; b4 < movelen; b4++)
        for(int b5 = 0; b5 < movelen; b5++)
        for(int b6 = 0; b6 < movelen; b6++)
        for(int b7 = 0; b7 < movelen; b7++)
        for(int b8 = 0; b8 < movelen; b8++)
        for(int b9 = 0; b9 < movelen; b9++)
        for(int ba = 0; ba < movelen; ba++)
        for(int bb = 0; bb < movelen; bb++)
        for(int bc = 0; bc < movelen; bc++)
        for(int bd = 0; bd < movelen; bd++)
        for(int be = 0; be < movelen; be++)
        for(int bf = 0; bf < movelen; bf++)
        {
                
          std::string str{ 
                      moves[a0],  
                      moves[a1],
                      moves[a2],
                      moves[a3],
                      moves[a4],
                      moves[a5],
                      moves[a6],
                      moves[a7],
                      moves[a8],
                      moves[a9],
                      moves[aa],
                      moves[ab],
                      moves[ac],
                      moves[ad],
                      moves[ae],
                      moves[af],
                      moves[ag],
                      moves[ah],
                      moves[ai],
                      moves[aj],
                      moves[ak],
                      moves[al],
                      moves[am],
                      moves[an],
                      moves[ao],
                      moves[ap],
                      moves[aq],
                      moves[ar],
                      moves[as],
                      moves[at],
                      moves[au],
                      moves[av],
                      moves[aw],
                      moves[ax],
                      moves[ay],
                      moves[az],
                      moves[b1],
                      moves[b2],
                      moves[b3],
                      moves[b4],
                      moves[b5],
                      moves[b6],
                      moves[b7],
                      moves[b8],
                      moves[b9],
                      moves[ba],
                      moves[bb],
                      moves[bc],
                      moves[bd],
                      moves[be],
                      moves[bf]
                    };
            auto pos = str.find_first_not_of("\0",0);
            of << str.substr(pos == std::string::npos ? str.length() + pos : pos) << std::endl;
            std::cout << str.substr(pos == std::string::npos ? str.length() + pos : pos) << std::endl;   
        }
        }
        
      
    }
     