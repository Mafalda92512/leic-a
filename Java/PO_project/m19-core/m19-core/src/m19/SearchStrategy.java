package m19;

import java.util.ArrayList;

public class SearchStrategy implements Strategy {

    private String _searchTerm;

    ArrayList<Work> _searchResults = new ArrayList<Work>();

    public SearchStrategy(String searchTerm){
        _searchTerm = searchTerm;
    }

    public void searchWork(Work work){
        if(!work.searchTitle(_searchTerm)){
            if(!work.searchAuthorDirector(_searchTerm))
            return;
        }
        _searchResults.add(work);
    }

    public ArrayList<Work> getResults(){
        return _searchResults;
    }


}