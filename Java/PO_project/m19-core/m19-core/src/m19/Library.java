package m19;

import java.util.TreeMap;
import java.util.ArrayList;
import java.util.Collections;

import java.io.Serializable;
import java.io.IOException;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileNotFoundException;

import m19.exceptions.BadEntrySpecificationException;
import m19.exceptions.UserNotFoundException;
import m19.exceptions.WorkNotFoundException;

/* Class that represents the library as a whole.
*/
public class Library implements Serializable {

  /** Serial number for serialization. */
  private static final long serialVersionUID = 201901101348L;

  /**
   * numUser is the id given to a created User
   */
  private int numUser;

  /**
   * numWork is the id given to a created Work
   */
  private int numWork;

  /**
   *The variable _date is the date of the Library.
   */
  private int _date;

  /**
   * The variable change, is one if the library has changed since the last save, 0 if not.
   */
  private int _change = 1;
  /**
   * TreeMap containing the User's name as key, and the User as value.
   */
  private TreeMap<Integer, User> _users = new TreeMap<Integer, User>();

  /**
   * TreeMap containing the Work's id as key, and the Work as value.
   */
  private TreeMap<Integer, Work> _works = new TreeMap<Integer, Work>();

  /**
   * ArrayList containing the requests of all the Library.
   */
  private ArrayList<Request> _requests = new ArrayList<Request>();


/*_________________________________CONSTRUTOR_________________________________*/

/**
 * Constructor for the class Library
 */
  public Library() {
    numUser = 0;
    numWork = 0;
    _date = 0;
  }
/*____________________________________________________________________________*/
/*______________________________GETTERS E SETTERS_____________________________*/

/**
 * This function returns the date of the Library.
 * @return int _date
 */
public int displayDate() {
  return _date;
}

/**
 * This funtion returns the variable _change, letting the LibraryManager know if the
 * Library has been changed since the last save.
 * @return int _change
 */
public int getChange(){
  return _change;
}

/**
 * Changes the value of the variable _change.
 * @param change this parameter is 0 if the library has just been saved, and 1 if the
 * Library has been changed.
 */
public void setChange(int change){
  _change = change;
}

/*____________________________________________________________________________*/
/*____________________________________USER____________________________________*/
  public User registerUser(String name, String email) {
    User user = new User(numUser, name, email);
    numUser++;
    _users.put(user.getId(), user);
    this.setChange(1);
    return user;
  }


  public User showUser(int userID) throws UserNotFoundException{
    if(!_users.containsKey(userID)) throw new UserNotFoundException();
    return _users.get(userID);
  }


  public ArrayList<User> showAllUsers() {
    ArrayList<User> listUsersSorted = new ArrayList<User>(_users.values());
    Collections.sort(listUsersSorted, User.userComparator);

    return listUsersSorted;
  }

/*____________________________________________________________________________*/
/*____________________________________WORK____________________________________*/


  public ArrayList<Work> displayAllWorks() {
    ArrayList<Work> worksList = new ArrayList<Work>();
    for(Work work: _works.values()){
      worksList.add(work);
    }
    return worksList;
  }


  public Work displayWork(int idWork) throws WorkNotFoundException{
    if (!_works.containsKey(idWork))
      throw new WorkNotFoundException();
    return _works.get(idWork);
  }


  public ArrayList<Work> performSearch(String searchTerm) {
    SearchStrategy _searchStrategy = new SearchStrategy(searchTerm);

    for (Work work : _works.values()) {
      _searchStrategy.searchWork(work);
    }
    return _searchStrategy.getResults();

  }

/*____________________________________________________________________________*/
/*__________________________________REQUESTS__________________________________*/

/**
 * This function makes the request of a work, to the user.
 * @param  user       The user who wants to request the work
 * @param  work       The work to be requested by the user
 * @param  returnTime The date until which the work must be returned
 * @return           returns 0 if the Request is successful, and the number of the failed rule
 *  if the request is not successful
 */
 public int requestWork(User user, Work work, int returnTime) {
  if (validateRequest(user, work) == 0) {
    Request r = new Request(returnTime, user, work);
    _requests.add(r);
    user.addRequest(work, r);
    work.decrementCopies();
    this.setChange(1);
    return 0;
  } else
    return validateRequest(user, work);
}

/**
 * This function returns the request a user made of a certain work
 * @param  user     The user specified
 * @param  work     The work specified
 * @return          Returns the request if it exists and a null pointer if not
 */
  public Request getRequest(User user, Work work){
    for(int i = 0; i<_requests.size(); i++){
      if (_requests.get(i).getUser().equals(user) && _requests.get(i).getWork().equals(work)) {
        return _requests.get(i);
      }
    }
    return null;
  }

  public void returnWork(User user, Work work) {
    int deliveryDay = -1;
    for (int i = 0; i < _requests.size(); i++) {
      if (_requests.get(i).getUser().equals(user) && _requests.get(i).getWork().equals(work)) {
        deliveryDay = _requests.get(i).getDeliveryDay();
        _requests.remove(i);
      }
    }
    user.removeRequest(work);
    if (_date > deliveryDay && deliveryDay != -1)
      user.badReturn();
    else
      user.goodReturn();
    work.incrementCopies();
    work.checkStatus();
    this.setChange(1);
  }

  public int validateRequest(User _user, Work _work) {
    Rule rules = new RuleComposite(_user, _work);
    return rules.test();
  }

  public void updateRequest(User user, Work work) {
    for (int i = 0; i < _requests.size(); i++) {
      if (_requests.get(i).getWork().equals(work) && _requests.get(i).getUser().equals(user))
        _requests.get(i).updateRequest(_date);
    }
  }

  public int returnTime(User user, int copies) {
    return user.calcReturnTime(copies);
  }

/*____________________________________________________________________________*/
/*___________________________________REGRAS___________________________________*/

  public Boolean isBorrowed(User user, Work work) {
    return user.hasWorkRequested(work);
  }
/*____________________________________________________________________________*/
/*________________________________NOTIFICACOES________________________________*/

  public ArrayList<Notification> showUserNotifications(int userID) throws UserNotFoundException{
    if(_users.get(userID) == null) throw new UserNotFoundException();
    return _users.get(userID).getNotifications();
  }


  public void cleanNotifications(int userID) {
    _users.get(userID).cleanUserNotifications();
    this.setChange(1);
  }


  public void requestsNotification(int workID, User userToNotify) {
    _works.get(workID).registerObserver(userToNotify);
  }

/*____________________________________________________________________________*/
/*___________________________________MULTA____________________________________*/

  public void payFine(User user) {
    user.payFine();
    this.setChange(1);
  }
/*____________________________________________________________________________*/
/*____________________________LIBRARY FUNCTIONS_______________________________*/

  /**
   * This funtion advances the date of the Library.
   * @param daysToAdvance The days to advance, which must be positive.
   */
  public void advanceDate(int daysToAdvance) {
    if (daysToAdvance > 0) {
      _date += daysToAdvance;
      this.setChange(1);
      for(int i = 0; i<_requests.size(); i++){
        if(_requests.get(i).getDeliveryDay() < _date) _requests.get(i).getUser().setSuspended();
        _requests.get(i).getUser().checkStatus();
      }
    }
  }

  /**
   * This method creates a DVD and adds it to the TreeMap containing Works and its
   * IDs.
   *
   * @param numCopies The number of Copies of the DVD
   * @param title     The Title of the DVD
   * @param price     The price of the DVD
   * @param category  The Category of the DVD
   * @param igac      The DVD's IGAC
   * @param director  The DVD's Director
   * @return returns the created DVD
   */
  public Work addDVD(int numCopies, String title, int price, String category, String igac, String director) {
    Work dvd = new DVD(numWork, numCopies, title, price, category, igac, director);
    numWork++;
    _works.put(dvd.getId(), dvd);
    return dvd;
  }

  /**
   * This method creates a Book and adds it to the TreeMap containing Works and
   * its IDs.
   *
   * @param numCopies The Book's copies
   * @param title     The Book's title
   * @param price     The Book's price
   * @param category  The Book's category
   * @param isbn      The Book's ISBN
   * @param author    The Book's author
   * @return returns the created Book
   */
  public Work addBook(int numCopies, String title, int price, String category, String isbn, String author) {
    Work book = new Book(numWork, numCopies, title, price, category, isbn, author);
    numWork++;
    _works.put(book.getId(), book);
    return book;
  }

  /**
   * Read the text input file at the beginning of the program and populates the
   * instances of the various possible types (books, DVDs, users).
   *
   * @param filename name of the file to load
   * @throws BadEntrySpecificationException
   * @throws IOException
   */
  void importFile(String filename) throws BadEntrySpecificationException, IOException {
    try {
      BufferedReader in = new BufferedReader(new FileReader(filename));
      String s;
      while ((s = in.readLine()) != null) {
        String line = new String(s.getBytes(), "UTF-8");
        if (line.charAt(0) == '#')
          continue;
        String[] split = line.split(":");
        switch (split[0]) {
        case "USER":
          this.registerUser(split[1], split[2]);
          break;
        case "DVD":
          this.addDVD(Integer.parseInt(split[6]), split[1], Integer.parseInt(split[3]), split[4], split[5], split[2]);
          break;
        case "BOOK":
          this.addBook(Integer.parseInt(split[6]), split[1], Integer.parseInt(split[3]), split[4], split[5], split[2]);
          break;
        }
      }
      in.close();
    } catch (FileNotFoundException e) {
       throw new BadEntrySpecificationException(filename, e);
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}

/*____________________________________________________________________________*/
