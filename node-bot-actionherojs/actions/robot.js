exports.movement = {
  name:                   'robotMovement',
  description:            'Action for moving the wheels via the motors for the robot.',
  blockedConnectionTypes: [],
  outputExample:          {},
  matchExtensionMimeType: false,
  version:                1.0,
  toDocument:             true,

  inputs: {
    required: ['velocityLeft', 'velocityRight'],
    optional: []
  },

  run: function(api, connection, next){
    
    api.robot.movement(connection.params.velocityLeft, connection.params.velocityRight, function(error){
      connection.error = error;
      next(connection, true);
    });

  }
};