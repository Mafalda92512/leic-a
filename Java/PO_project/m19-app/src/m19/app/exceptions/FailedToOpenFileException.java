package m19.app.exceptions;

import pt.tecnico.po.ui.DialogException;

public class FailedToOpenFileException extends DialogException {

    /** Serial number for serialization. */
    static final long serialVersionUID = 201901091828L;

    /**
     * Bad file name.
     */
    private String _name;

    /**
     * @param name
     */
    public FailedToOpenFileException(String name) {
        _name = name;
    }

    /** @see pt.tecnico.po.ui.DialogException#getMessage() */
    public String getMessage() {
        return Message.fileNotFound(_name);
    }

}
