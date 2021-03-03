package m19;

public class Book extends Work {
  /**
   *
   */
  private static final long serialVersionUID = -905035085979302479L;
  private String _ISBN;
  private String _author;

  public Book(int idWork,  int numCopies, String title, int price, String category, String ISBN, String author){
    super(idWork, numCopies, title, price, category);
    _ISBN = ISBN;
    _author = author;
  }

  public String getISBN(){
    return _ISBN;
  }

  public String getAuthor(){
    return _author;
  }

  @Override
  public String toString(){
    return super.toString1()+ " - Livro - " + super.toString2()+ " - " +this.getAuthor()+" - "+this.getISBN();
  }

  @Override
  public Boolean searchAuthorDirector(String searchTerm) {
    return this.getAuthor().toLowerCase().contains(searchTerm.toLowerCase());
  }

  @Override
  public Boolean searchTitle(String searchTerm) {
    return this.getTitle().toLowerCase().contains(searchTerm.toLowerCase());
  }

}
