def tictactoe_win_masks():
    win_masks = []

    row_mask = 0b111
    col_mask = 0b001001001
    for i in range(3):
        win_masks.append("0x%02x" % row_mask)
        win_masks.append("0x%02x" % col_mask)
        row_mask <<= 3
        col_mask <<= 1

    win_masks.append("0b001010100")
    win_masks.append("0b100010001")

    return ', '.join(win_masks)


if __name__ == '__main__':
    print(tictactoe_win_masks())