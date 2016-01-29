(function ($) {
    $(document).ready(function(){
       
        var watchPositionID;
        var positionLastUpdate = 0;
        
        if(navigator.geolocation) {
            watchPositionID = navigator.geolocation.watchPosition(positionSuccessCallback, positionErrorCallback, {
                timeout: 30000,
                maximumAge: 20000,
                enableHighAccuracy: true
            });
        }
        
        function positionSuccessCallback(position) {
            
            // on ne met à jour la position que toutes les 20s.
            if(position['timestamp'] - positionLastUpdate < 20000) {
                return;
            }

            document.cookie = "sol_geoloc_position_lng=" + position.coords.longitude + "; path=/";
            document.cookie = "sol_geoloc_position_lat=" + position.coords.latitude + "; path=/";
            
            positionLastUpdate = position['timestamp'];
        }
        
        function positionErrorCallback(error) {
            navigator.geolocation.clearWatch(watchPositionID);
        }
        
    });    
})(jQuery);