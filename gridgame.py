from random import randint

def gridgame(p):
    # Board size
    max = (3, 3)

    # Remember the last move for each player
    lastmove = [-1, -1]

    # Remember the player's locations
    location = [[randint(0, max[0]), randint(0, max[1])]]

    # Put the second player a sufficient distance from the first
    location.append([(location[0][0] + 2) % 4, (location[0][1] + 2) % 4])
    # Maximum of 50 moves before a tie
    for o in range(50):

        # For each player
        for i in range(2):
            locs = location[i][:] + location[1 - i][:]
            locs.append(lastmove[i])
            move = p[i].evaluate(locs) % 4

            # You lose if you move the same direction twice in a row
            if lastmove[i] == move: return 1 - i
            lastmove[i] = move
            if move == 0:
                location[i][0] -= 1
                # Board wraps
                if location[i][0] < 0: location[i][0] = 0
            if move == 1:
                location[i][0] += 1
                if location[i][0] > max[0]: location[i][0] = max[0]
            if move == 2:
                location[i][1] -= 1
                if location[i][1] < 0: location[i][1] = 0
            if move == 3:
                location[i][1] += 1
                if location[i][1] > max[1]: location[i][1] = max[1]

            # If you have captured the other player, you win
            if location[i] == location[1 - i]: return i
    return -1


def tournament(playerlist):
    # Count losses
    losses = [0 for p in playerlist]

    # Every player plays every other player
    for i in range(len(playerlist)):
        for j in range(len(playerlist)):
            if i == j: continue

            # Who is the winner?
            winner = gridgame([playerlist[i], playerlist[j]])

            # Two points for a loss, one point for a tie
            if winner == 0:
                losses[j] += 2
            elif winner == 1:
                losses[i] += 2
            elif winner == -1:
                losses[i] += 1
                losses[j] += 1
                pass

    # Sort and return the results
    z = zip(losses, playerlist)
    z.sort()
    return z


class humanplayer:
    def evaluate(self, board):

        # Get my location and the location of other players
        me = tuple(board[0:2])
        others = [tuple(board[x:x + 2]) for x in range(2, len(board) - 1, 2)]

        # Display the board
        for i in range(4):
            for j in range(4):
                if (i, j) == me:
                    print 'O',
                elif (i, j) in others:
                    print 'X',
                else:
                    print '.',
            print

        # Show moves, for reference
        print 'Your last move was %d' % board[len(board) - 1]
        print ' 0'
        print '2 3'
        print ' 1'
        print 'Enter move: ',

        # Return whatever the user enters
        move = int(raw_input())
        return move

