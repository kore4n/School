package CS2212.group21;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class SearchHelper {
    Boolean adminPermissions;
    JSONArray wholeBuilding;
    JSONObject tmpObj;
    public SearchHelper(JSONArray allBuildings, Boolean admin){
        this.adminPermissions = admin;
        this.wholeBuilding = allBuildings;
    }

    public String getBuildIndex(int indexValue){
        tmpObj = (JSONObject) wholeBuilding.get(indexValue);
        return tmpObj.get("Building").toString();

    }
    public int getPOIIndex(JSONArray poiList, String name, String roomNum){
        for (int n = 0; n < poiList.size(); n++){
            tmpObj = (JSONObject) poiList.get(n);
            if (tmpObj.get("name").equals(name) && tmpObj.get("roomNum").equals(roomNum)) {
                    return n;
            } else {
                String tmpString = "(User)" + tmpObj.get("name").toString();
                if(tmpString.equals(name) && tmpObj.get("roomNum").equals(roomNum)){
                    return n;
                }
            }
        }
        return -1;
    }

    public int getCord(String XY, JSONObject poi){
        if(XY.equals("X")){
            String string = poi.get("xCord").toString();
            int i = Integer.parseInt(string);
            return i;
        } else {
            String string = poi.get("yCord").toString();
            int i = Integer.parseInt(string);
            return i;
        }
    }
    public JSONObject getBuildObject(String buildName){
        String tmp = "";
        for (int n = 0; n < wholeBuilding.size(); n++){
            tmpObj = (JSONObject) wholeBuilding.get(n);
            if (tmpObj.get("Building").equals(buildName)){ return tmpObj; }
        }
        return null;
    }
    public String findImage(String buildName, int floorNum){
        tmpObj = (JSONObject) getBuildObject(buildName);
        JSONArray tmpArray = (JSONArray) tmpObj.get("floors");
        tmpObj = (JSONObject) tmpArray.get(floorNum);
        return tmpObj.get("imageFileName").toString();
    }

    public JSONArray findAllLayerType(JSONObject currFloor, String layerType){
        JSONArray tmpArray = (JSONArray) currFloor.get("pointsOfInterest");
        JSONArray returnList = new JSONArray();
        JSONObject tmpObj;
        String tmpString = "";
        for (int n = 0; n < tmpArray.size(); n++){
            tmpObj = (JSONObject) tmpArray.get(n);
            tmpString = tmpObj.get("layerType").toString();
            if (tmpString.equals(layerType) == true){
                returnList.add(tmpObj);
            }
        }
        return returnList;
    }
}
