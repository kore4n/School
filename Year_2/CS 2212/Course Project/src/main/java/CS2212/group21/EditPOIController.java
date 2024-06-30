package CS2212.group21;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import java.net.URL;
import java.util.ResourceBundle;

public class EditPOIController implements Initializable {
    @FXML
    public TextField name;
    @FXML
    public TextField desc;
    @FXML
    public TextField layerType;
    @FXML
    public TextField roomNum;
    @FXML
    public TextField x;
    @FXML
    public TextField y;
    @FXML
    public Button button_login;
    @FXML
    public Button button_login1;
    public static String pname;
    public static String pdesc;
    public static String proom;
    public static String player;

    private void submit(ActionEvent event) {
        pname = name.getText();
        pdesc = desc.getText();
        proom = roomNum.getText();
        player = layerType.getText();
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
