exports.robot = function(api, next){

  api.robot = {

    movement: function(velocityLeft, velocityRight, next){
 	  console.log('robot.movement: ' + velocityLeft + ' | ' + velocityRight);

 	  // Error exists? Pass it to next().
 	  var error = null;
 	  next(error);
    }

  };

  api.robot._start = function(api, next){
    next();
  };

  api.robot._stop =  function(api, next){
    next();
  };

  next();
}