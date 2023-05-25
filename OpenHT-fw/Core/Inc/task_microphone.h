/*
 * TaskMicrophone.h
 *
 *  Created on: May 22, 2023
 *      Author: morga
 */

#ifndef INC_TASK_MICROPHONE_H_
#define INC_TASK_MICROPHONE_H_

void StartMicrophonesTask(void *argument);

void start_microphone_acquisition();
void stop_microphone_acquisition();

#endif /* INC_TASK_MICROPHONE_H_ */
