package m19.app.users;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import m19.User;
import m19.app.exceptions.NoSuchUserException;

import m19.exceptions.UserNotFoundException;



/**
 * 4.2.2. Show specific user.
 */
public class DoShowUser extends Command<LibraryManager> {

  private Input<Integer> _userID;

  /**
   * @param receiver
   */
  public DoShowUser(LibraryManager receiver) {
    super(Label.SHOW_USER, receiver);
    _userID = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try{
      String  user = _receiver.showUser(_userID.value()).toString();
      _display.popup(user);
    }
    catch(UserNotFoundException e){
      throw new NoSuchUserException(_userID.value());
    }
  }

}
