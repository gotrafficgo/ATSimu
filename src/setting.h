#pragma once

/////CA-model
#define DEFAULT_MAX_LANE_CELL_NUMBER_A                        500          //CF--5000      //NS--500
#define DEFAULT_METER_PER_CELL_A                                      7               //CF--1           //NS--7          //the length of a cell in reality. one cell: 4 meters
#define DEFAULT_CELL_LENGTH_A                                           8               //CF--1           //NS--8         //cell length in screen  
#define DEFAULT_P                                                              0.01  
#define DEFAULT_P2                                                            0.5
#define ENTERING_BOUNDRY_A                                           6                //see comments of Entering_Boundry
#define CHECKING_REGION_A                                             14              //see comments of Checking_Region

/////CF-model
#define DEFAULT_MAX_LANE_CELL_NUMBER_F                        5000          //CF--5000      //NS--500
#define DEFAULT_METER_PER_CELL_F                                      1               //CF--1           //NS--7          //the length of a cell in reality. one cell: 4 meters
#define DEFAULT_CELL_LENGTH_F                                           1               //CF--1           //NS--8         //cell length in screen  
#define ENTERING_BOUNDRY_F                                                6     //convert from CA     //see comments of Entering_Boundry
#define CHECKING_REGION_F                                                   (14*7)        //see comments of Checking_Region   

//size of network
#define MAX_MARGINALPOINT_NUMBER                50
#define MAX_CROSS_NUMBER                              100
#define MAX_LINK_NUMBER                                   200    

//Link    
#define LANE_WIDTH                                             8 
#define MAX_LANE_NUMBER                                  5           ////max lane number in one link
#define MAX_ORIGIN_NUMBER                             10

//Intersection
#define MAX_CROSS_LANE_CELL_NUMBER          100        // max cell # in a crossing lane
#define MAX_CROSS_LANE_NUMBER                   100       //max lane # in a crossing
#define RADIUS_PERCENT_OF_DISTANCE             0.8       //the fraction of radius and chord used in drawing the outline of crossing
#define MAX_CROSS_POINT_NUMBER                  8            //max point # in the outline of crossing 
#define ALL_IN_ONE                                            3             //conflicts related, size of conflict region 
#define MAX_ENTER_LINK                                    4             //4 links intersection; i.e., crossing
#define MAX_CONFLICT_CELL_NUMBER              10           //max cell number in a conflict area
#define MAX_CONFLICT_AREA_NUMBER             75           //max conflict number in a crossing
#define MAX_CONFLICT_AREA_ID_NUMBER        20           //a conflict cell in left-turn lane belongs to how many conflict area maximumly.
#define SPEED_IN_CROSS                                   1            //veh has a constant speed in crossing
#define LEFT_CONFLICT_SCOPE                          1            //2       //search conflicted or not in this scope 
#define TROUGH_CONFLICT_SCOPE                   3             //3
#define CELL_LENGTH_IN_CROSS                        8           
#define INTERVAL_IN_CHORD                             CELL_LENGTH_IN_CROSS

//driver behavior
#define MAX_DEST_LANE_NUMBER                     MAX_LANE_NUMBER   //max destination lane number
#define MAX_SPEED_PERCENT_1                               1    
#define MAX_SPEED_PERCENT_2                               0.5
#define MAX_SPEED_PERCENT_3                               1
#define MAX_WAITING_TIME                                    10        //after this time, drivers change their routes.
#define PROB_OF_CHANGE_LANE                          (1.0/2)    //(1.0/4), changing line if one has 4 chances,  from 0.25, increase to 0.5
#define MIN_LINK_NUMBER                                      3          //one route is composed of 3 links at least
#define VEHICLE_LENGTH                                        7

//sign on the link
#define PROHIBIT_CHANGE_LANE_DISTANCE            35        //within this distance, lane-change is not allowed
#define GUIDANCE_LOCATION			                       50        //the location of guidance board
#define GUIDANCE_BOARD_RADUS                          8         //the size of guidance sign shown in the screen
#define QUEUE_DETECTOR_LOCATION                      29        
#define CROSS_DETECTOR_LOCATION                       0        
#define LONG_TIME_WAIT_REGION                     (PROHIBIT_CHANGE_LANE_DISTANCE+35)    //in this range, veh will check if long time waiting. consider Wait_Cell_ID, so it is 5

//ITS device
#define MAX_PHASE_NUMBER                                 6
#define GUIDANCE_CIRCLE                                    200
// #define DETECTOR_SCOPE                                       (MAX_VEH_SPEED_1+1) 
#define DETECTOR1_TIMES                                        'a' 
#define DETECTOR2_TIMES                                        'b' 
#define DETECTOR_IN_BEGINNING                         -11
#define LINK_IN_ONE_PHASE                                2 
#define NO_DETECTOR_ROAD_ID_1                        99
#define NO_DETECTOR_ROAD_ID_2                       142
#define MAX_DETECTOR_NUMBER                         3 
#define ALL_GREEN                                                   100
#define START_TIME_ALL_GREEN                             0 
#define INIT_CONTROL_CYCLE                                30
#define MAX_CONTROL_DETECTOR                       12
#define FAKE_GUIDANCE_INFO                              -111
#define NCURVE_RECORD_FREQ                           20     //How many secs does it record veh count.
#define SCAN_REGION                                         8+1       //detector compares this region every time

//other
#define VEH_TYPE_NUMBER                     3            //the number of veh types
#define DRIVER_TYPE_NUMBER                       3            //the number of driver types
#define DEVIATION_XML                                  0            //the beginning number of serial number in XML
#define SIMU_STEP_LENGTH                            100         //simulation step length; note: if no sleep, it is around 60
#define MAX_ROUTE_LENGTH                         30           //max link number in a route  //original is 50
#define MAX_LANE_INFO                                12
#define MAX_GUIDANCE_DESTINATION       5 
#define MAX_TURNING_DIRECTION              5            
#define CONTROL_BUFFER_SIZE                     768          //receiving buffer size
#define VEH_SIZE                                      3              //for drawing
#define LIGHT_SIZE                                           4              //for drawing
#define MAX_LOG_FILE_SUM                       15               //max generated log file number 
#define INITIAL_VALUE_OF_LAST_REFRESH_TIME           -1    
#define SLEEP_TIME                                           0        
#define A_MIN                                                   60   
#define BROADCASTING_FREQUENCY           5              //the time interval that guidance information is blinkcasted.
#define MAX_DEMAND_NUMBER                  24            //24 hours per day
#define MAX_DEMAND                                60            //vehicles are generated in every sec.
#define MAX_FixedOD_NUMBER                           20          //the max number of the OD get from XML
#define FREE_FLOW_VEH_NUMBER               10          //used in calculating link info for travel time shortest path. If the veh number is less than this number, we consider the state is free flow, no matter what is the speed.
#define MAX_ITERATION_NUMBER                  20        //to prevent from involving a long time iteration from "while"
#define MAXNUM                                        1111111111
#define TIME_INTERVAL_DRAW_MFD              1
#define ARRIVAL_SCOPE                                 5      //to information commuters, 	//if the travel time predicted by this tsp can guarantee commuters' arrival at the [Required_Arrival_Time - ARRIVAL_SCOPE, Required_Arrival_Time - ARRIVAL_SCOPE], depart!


//show area
#define OrigEllipseCX                         14000              //the size of dragging and drawing area       7000
#define OrigEllipseCY                         -10000             //the size of dragging and drawing area     -5000
#define SIZEOFF_X                                2000                 //displacement of network drawn to the drawing area        1150
#define SIZEOFF_Y							     2500                 //displacement of network drawn to the drawing area         1500
#define OrigSizeOff_X                         -2000              //displacement of drawing area to screen    -1000
#define OrigSizeOff_Y                          4000               //displacement of drawing area to screen     2000

//color
#define BACKGROUND_COLOR           COLOR_BLACK 
#define NETWORK_COLOR                    COLOR_WHITE  
#define VEH_COLOR                             COLOR_BLACK 
#define MIDDLELINE_COLOR                RGB(255,128,0)  
#define BLANK_COLOR                          LINK_COLOR 
#define CROSS_COLOR                          NETWORK_COLOR
#define COLOR_GREEN                          RGB(0,255,0)
#define COLOR_SAP_GREEN                 RGB(64,128,128)
#define COLOR_YELLOW                        RGB(255,255,0)
#define COLOR_RED			                      RGB(255,0,0)
#define COLOR_BLUE                             RGB(0,0,128)
#define COLOR_WHITE                            RGB(255,255,255)
#define COLOR_BLACK                          RGB(0,0,0)
#define COLOR_GREY                             RGB(128,128,128)
#define COLOR_GREEN_VEH	              RGB(0,255,255)
#define COLOR_RED_VEH                     RGB(128,0,0)
#define COLOR_BLUE_VEH	                 COLOR_BLUE
#define LINK_COLOR                            RGB(120,120,120)
#define CORRIDOR_COLOR                   RGB(70,70,70)

//communication
#define GUIDANCE_PORT                5555
#define CONTROL_PORT                 6666
#define FLOW_DATA_PORT    	     7777
#define SIMU_TIME_PORT               8888
#define   WM_UPDATEDATA   WM_USER + 5

//traffic flow theory
// #define FREE_FLOW_SPEED          8              //3cell/s=12m/s    free flow speed: 100/3=33.3(m/s)                    recycle:    double(33.33/(2*Meter_Per_Cell))
// #define WAVE_SPEED                  1                //1cell/s=4m/s     wave speed: 50.0/9=5.56(m/s)             recycle:   double(5.56/(2*Meter_Per_Cell)) 
// #define MAX_DENSITY                  1                // 1veh/cell         max density: 3.0/20=0.15(veh/m)                       recycle:   double(0.15*Meter_Per_Cell) 


//commuters
#define MAX_COMMUTER_NUMBER_PER_ORIGIN    100
#define MAX_SIMU_DAY_NUMBER    20    //max number of simulated days


