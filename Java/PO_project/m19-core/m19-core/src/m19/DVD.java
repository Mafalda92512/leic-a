package m19;


public class DVD extends Work {
  /**
   *
   */
  private static final long serialVersionUID = -3961373473358449497L;
  private String _IGAC;
  private String _director;


  public DVD(int idWork,  int numCopies, String title, int price, String category, String IGAC, String director){
    super(idWork, numCopies, title, price, category);
    _IGAC = IGAC;
    _director = director;
  }
  
  public String getDirector(){
    return _director;
  }

  public String getIGAC(){
    return _IGAC;
  }

  @Override
  public String toString(){
    return super.toString1()+ " - DVD - " + super.toString2()+" - "+ this.getDirector()+" - "+this.getIGAC();
  }

  @Override
  public Boolean searchAuthorDirector(String searchTerm){
    return this.getDirector().toLowerCase().contains(searchTerm.toLowerCase());
  }

  @Override
  public Boolean searchTitle(String searchTerm){
    return this.getTitle().toLowerCase().contains(searchTerm.toLowerCase());
  }

}
