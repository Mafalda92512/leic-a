package m19.app.works;

import pt.tecnico.po.ui.Command;

import m19.LibraryManager;
import m19.Work;
import m19.Book;
import m19.DVD;


/**
 * 4.3.2. Display all works.
 */
public class DoDisplayWorks extends Command<LibraryManager> {
  /**
   * @param receiver
   */
  public DoDisplayWorks(LibraryManager receiver) {
    super(Label.SHOW_WORKS, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() {
    for (Work work : _receiver.displayAllWorks()){
        _display.addLine(work.toString());
      }
      _display.display();
  }

  }
