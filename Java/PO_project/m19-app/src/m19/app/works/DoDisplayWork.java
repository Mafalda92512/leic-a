package m19.app.works;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.Input;
import pt.tecnico.po.ui.DialogException;

import m19.Work;
import m19.Book;
import m19.DVD;
import m19.app.exceptions.NoSuchWorkException;

import m19.exceptions.WorkNotFoundException;


/**
 * 4.3.1. Display work.
 */
public class DoDisplayWork extends Command<LibraryManager> {

  private Input<Integer> _workID;

  /**
   * @param receiver
   */
  public DoDisplayWork(LibraryManager receiver) {
    super(Label.SHOW_WORK, receiver);
    _workID = _form.addIntegerInput(Message.requestWorkId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try{
    _display.popup(_receiver.displayWork(_workID.value()).toString());
  }
    catch(WorkNotFoundException e){
      throw new NoSuchWorkException(_workID.value());
    }
}

}
