package m19.app.users;

import m19.LibraryManager;
import m19.User;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

import m19.app.exceptions.UserIsActiveException;
import m19.app.exceptions.NoSuchUserException;

import m19.exceptions.UserNotFoundException;


/**
 * 4.2.5. Settle a fine.
 */
public class DoPayFine extends Command<LibraryManager> {

  private Input<Integer> _userID;

  /**
   * @param receiver
   */
  public DoPayFine(LibraryManager receiver) {
    super(Label.PAY_FINE, receiver);
    _userID = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      User user = _receiver.showUser(_userID.value());
      if(!user.isSuspended()) 
        throw new UserIsActiveException(_userID.value());
      _receiver.payFine(user);
    }
     catch (UserNotFoundException e) {
      throw new NoSuchUserException(_userID.value());
    }
  }

}
