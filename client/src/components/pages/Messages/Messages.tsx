import React from "react";

type Props = {
  id: string;
};

function Messages({ id }: Props) {
  return <div id={id}>Messages</div>;
}

export default Messages;
