DROP TABLE IF EXISTS Observer cascade;
CREATE TABLE Observer (user_id SERIAL NOT NULL,
username CHAR(256),
password CHAR(256),
email CHAR(256),
user_project_desc TEXT,
quota BIGINT,
priority INT,
subscription_date TIMESTAMP WITH TIME ZONE,
PRIMARY KEY (user_id) );

DROP TABLE IF EXISTS Observation_sequence ;
CREATE TYPE obs_state AS ENUM ('pending','running','done','outdated');
CREATE TABLE Observation_sequence (observation_id SERIAL NOT NULL,
observation_state OBS_STATE,
exposure_time INT,
observation_attempts SMALLINT,
request_id SERIAL NOT NULL,
PRIMARY KEY (request_id, observation_id) );

DROP TABLE IF EXISTS Request ;
CREATE TABLE Request (request_id SERIAL NOT NULL,
request_name CHAR(256),
request_target CHAR(256),
target_dec FLOAT,
target_ra FLOAT,
request_state OBS_STATE,
duration INT,
observation_interval_start TIMESTAMP,
observation_interval_end TIMESTAMP,
observation_repetition INT,
observation_period BIGINT,
minimum_height FLOAT,
minimum_moon_distance FLOAT,
PRIMARY KEY (request_id) );


CREATE TYPE equipment_state AS ENUM('operationnal','maintenance','broken');

DROP TABLE IF EXISTS Telescope ;
CREATE TABLE Telescope (telescope_id SERIAL NOT NULL,
telescope_reference CHAR(256),
telescope_name CHAR(256),
telescope_mounting CHAR(256),
focal_distance FLOAT,
telescope_opening FLOAT,
telescope_state EQUIPMENT_STATE,
camera_id SERIAL,
PRIMARY KEY (telescope_id) );

DROP TABLE IF EXISTS Camera ;
CREATE TABLE Camera (camera_id SERIAL NOT NULL,
camera_reference CHAR(256),
camera_name CHAR(256),
camera_sensor CHAR(256),
sensor_size FLOAT,
pixel_size FLOAT,
camera_gain FLOAT,
camera_state EQUIPMENT_STATE,
telescope_id SERIAL,
PRIMARY KEY (camera_id) );

DROP TABLE IF EXISTS Filter_type ;
CREATE TABLE Filter_type (filter_type_id SERIAL NOT NULL,
filter_type_name CHAR(256),
filter_type_description TEXT,
PRIMARY KEY (filter_type_id) );

DROP TABLE IF EXISTS Filter ;
CREATE TABLE Filter (filter_id SERIAL NOT NULL,
filter_state EQUIPMENT_STATE,
filter_reference CHAR(256),
filter_name CHAR(256),
filter_type_id BIGINT,
telescope_id SERIAL,
PRIMARY KEY (filter_id) );

DROP TABLE IF EXISTS Alert ;
CREATE TABLE Alert (alert_id SERIAL NOT NULL,
alert_type CHAR(256),
alert_name CHAR(256),
alert_description TEXT,
PRIMARY KEY (alert_id) );

/*********************************** Associations *************************************/

DROP TABLE IF EXISTS observer_request ;
CREATE TABLE observer_request (user_id SERIAL NOT NULL,
request_id SERIAL NOT NULL,
submission_date TIMESTAMP WITH TIME ZONE,
PRIMARY KEY (user_id,
 request_id) );

DROP TABLE IF EXISTS observation_telescope ;
CREATE TABLE observation_telescope (observation_id SERIAL NOT NULL,
request_id SERIAL NOT NULL,
telescope_id SERIAL NOT NULL,
observation_date_start TIMESTAMP,
observation_date_end TIMESTAMP,
PRIMARY KEY (request_id, observation_id,
 telescope_id) );

DROP TABLE IF EXISTS observation_filter ;
CREATE TABLE observation_filter (filter_type_id SERIAL NOT NULL,
request_id SERIAL NOT NULL,
observation_id SERIAL NOT NULL,
filter_user_exposure_time INT,
PRIMARY KEY (filter_type_id,
 request_id, observation_id) );

/*Done part*/

DROP TABLE IF EXISTS alert_request ;
CREATE TABLE alert_request (request_id SERIAL NOT NULL,
alert_id SERIAL NOT NULL,
alert_date TIMESTAMP WITH TIME ZONE,
PRIMARY KEY (request_id,
 alert_id) );


ALTER TABLE Observation_sequence ADD CONSTRAINT FK_Observation_sequence_request_id FOREIGN KEY (request_id) REFERENCES Request (request_id);
ALTER TABLE Telescope ADD CONSTRAINT FK_Telescope_camera_id FOREIGN KEY (camera_id) REFERENCES Camera (camera_id);
ALTER TABLE Camera ADD CONSTRAINT FK_Camera_telescope_id FOREIGN KEY (telescope_id) REFERENCES Telescope (telescope_id);
ALTER TABLE Filter ADD CONSTRAINT FK_Filter_filter_type_id FOREIGN KEY (filter_type_id) REFERENCES Filter_type (filter_type_id);
ALTER TABLE Filter ADD CONSTRAINT FK_Filter_telescope_id FOREIGN KEY (telescope_id) REFERENCES Telescope (telescope_id);
ALTER TABLE observer_request ADD CONSTRAINT FK_user_request_user_id FOREIGN KEY (user_id) REFERENCES Observer (user_id);
ALTER TABLE observer_request ADD CONSTRAINT FK_user_request_request_id FOREIGN KEY (request_id) REFERENCES Request (request_id);
ALTER TABLE observation_telescope ADD CONSTRAINT FK_observation_telescope_observation_id FOREIGN KEY (request_id, observation_id) REFERENCES Observation_sequence (request_id, observation_id);
ALTER TABLE observation_telescope ADD CONSTRAINT FK_observation_telescope_telescope_id FOREIGN KEY (telescope_id) REFERENCES Telescope (telescope_id);
ALTER TABLE observation_filter ADD CONSTRAINT FK_observation_filter_filter_type_id FOREIGN KEY (filter_type_id) REFERENCES Filter_type (filter_type_id);
ALTER TABLE observation_filter ADD CONSTRAINT FK_observation_filter_observation_id FOREIGN KEY (request_id, observation_id) REFERENCES Observation_sequence (request_id, observation_id);
ALTER TABLE alert_request ADD CONSTRAINT FK_alert_request_request_id FOREIGN KEY (request_id) REFERENCES Request (request_id); 
ALTER TABLE alert_request ADD CONSTRAINT FK_alert_request_alert_id FOREIGN KEY (alert_id) REFERENCES Alert (alert_id);