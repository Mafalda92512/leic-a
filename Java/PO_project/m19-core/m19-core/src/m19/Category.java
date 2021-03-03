package m19;

public enum Category{
  REFERENCE("Referência"), FICTION("Ficção"), SCITECH("Técnica e Científica");

  private String _category;

  private Category(String category){
    _category = category;
  }

  public String getCategoryEnum(){ return _category; }
}
