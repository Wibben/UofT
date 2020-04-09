#
# Copyright 2020 University of Toronto
#
# Permission is hereby granted, to use this software and associated
# documentation files (the "Software") in course work at the University
# of Toronto, or for personal use. Other uses are prohibited, in
# particular the distribution of the Software either publicly or to third
# parties.
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

import select
from senderbase import SenderBase
from ece361.lab2.frame import Frame

class SlidingWindowSender(SenderBase):
    def __init__(self, file, destination, frame_size, timeout, maxseqnum, sender_window_size):
        super().__init__(file, destination, frame_size, timeout, maxseqnum)
        self.send_queue = []
        self.sender_window_size = sender_window_size
        self.maxseqnum = maxseqnum

    def _arqsend(self):
        last_acked = 0
        # implementation of the Sliding Window ARQ protocol
        while True:
            # a queue is the perfect data structure for implementing sliding window
            while (len(self.send_queue) < self.sender_window_size):
                # fill up the sending window
                current_frame = self.get_next_frame()
                # add frame to queue
                if SenderBase.ENABLE_DEBUG:
                    print('ADDING -', current_frame.seqnum,current_frame.data)
                self.send_queue.append(current_frame)

            if (self.send_queue[0].data == b''):
                # end of file
                break

            ''' Part 2: Implement the sliding window ARQ protocol:'''

            ''' Step 1: Go through the send queue and send those frames that need to be sent/resent'''
            ''' Do not forget to update the frames_sent variable'''
            for current_frame in self.send_queue:
                if (current_frame.data == b''):
                    break
                # send the new frames
                current_frame.send()
                if SenderBase.ENABLE_DEBUG:
                    print('SENDING -', current_frame.seqnum, current_frame.data)
                #update frames sent
                self.frames_sent += 1
            
            ''' Step 2: wait on all frames in the send_queue in parallel'''
            ''' Hint: call the wait_for_multiple_ack_nacks function'''
            Frame.wait_for_multiple_ack_nacks(self.send_queue)
            
            ''' Step 3: Go through the send queue and process ack/nack/timeout'''
            ''' Use the status() method to check status of the frame'''
            ''' Hint: In the sliding window ARQ if a frame is acked it is implied that all earlier frames are also acked even without receiving an explicit ack(why?)'''
            for current_frame in reversed(self.send_queue):
            #for current_frame in self.send_queue:
                if (current_frame.status() == Frame.Status.ack_nacked):
                    # retrieve the acknowledgement frame, which is a frame with only sequence number and no data
                    ack_frame = current_frame.retrieve_ack_nack()
                    
                    # print the original message, the ACK/NACK and the RTT
                    if SenderBase.ENABLE_DEBUG:
                        print(current_frame.data,
                              'DELIVERED. ACK:', ack_frame.seqnum,
                              'RTT:', current_frame.acktime() - current_frame.sendtime())
                    
                    # update last_acked frame
                    last_acked = ack_frame.seqnum
                    break
                elif (current_frame.status() == Frame.Status.timedout):
                    # timedout
                    if SenderBase.ENABLE_DEBUG:
                        print('TIMEOUT -', current_frame.seqnum, current_frame.data)
                else:
                    # frame is still in flight. do nothing.
                    if SenderBase.ENABLE_DEBUG:
                        print('INFLIGHT -', current_frame.seqnum, current_frame.data)

            ''' Step 4: Remove acked frames by calling the 'pop' method on the send queue'''
            ''' In the sliding window ARQ there might be more than 1 acked frames in an iteration'''
            ''' so you might need to call 'pop' multiple times'''
            ''' Do not forget to update the frames_delivered variable'''
            
            while (self.send_queue!=[] and self.send_queue[0].seqnum!=last_acked):
                if SenderBase.ENABLE_DEBUG:
                    print('POPPING -', self.send_queue[0].seqnum,self.send_queue[0].data)
                self.send_queue.pop(0)
                self.frames_delivered += 1






