/* Undefined instructions */
					.global SERVICE_UND
SERVICE_UND:                                
                    B   SERVICE_UND         
/* Software interrupts */
					.global SERVICE_SVC
SERVICE_SVC:                                
                    B   SERVICE_SVC         
/* Aborted data reads */
					.global SERVICE_ABT_DATA
SERVICE_ABT_DATA:                           
                    B   SERVICE_ABT_DATA    
/* Aborted instruction fetch */
					.global SERVICE_ABT_INST
SERVICE_ABT_INST:                           
                    B   SERVICE_ABT_INST   
					.global SERVICE_FIQ 
SERVICE_FIQ:                                
                    B   SERVICE_FIQ         
                    
                    .end                                    
