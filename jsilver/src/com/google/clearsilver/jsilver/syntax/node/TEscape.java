/* This file was generated by SableCC (http://www.sablecc.org/). */

package com.google.clearsilver.jsilver.syntax.node;

import com.google.clearsilver.jsilver.syntax.analysis.*;

@SuppressWarnings("nls")
public final class TEscape extends Token
{
    public TEscape()
    {
        super.setText("escape");
    }

    public TEscape(int line, int pos)
    {
        super.setText("escape");
        setLine(line);
        setPos(pos);
    }

    @Override
    public Object clone()
    {
      return new TEscape(getLine(), getPos());
    }

    public void apply(Switch sw)
    {
        ((Analysis) sw).caseTEscape(this);
    }

    @Override
    public void setText(@SuppressWarnings("unused") String text)
    {
        throw new RuntimeException("Cannot change TEscape text.");
    }
}
