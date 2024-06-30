package CS2212.group21;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import java.net.URL;
import java.util.ResourceBundle;

public class AddFloorController implements Initializable {
    @FXML
    private TextField floor;
    @FXML
    public Button button_login;
    @FXML
    public Button button_login1;

    private void submit(ActionEvent event) {
        System.out.println(floor.getText());
        Stage stage = (Stage)button_login.getScene().getWindow();
        stage.close();
    }

    private void cancel(ActionEvent event){
        Stage stage = (Stage)button_login1.getScene().getWindow();
        stage.close();
    }
    @Override
    public void initialize(URL url, ResourceBundle rb){
        button_login.setOnAction(this::submit);
        button_login1.setOnAction(this::cancel);
    }
}
