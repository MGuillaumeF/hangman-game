import React from 'react'

type Props = {
    id : string;
}

function Home({id} : Props) {
  return (
    <div id={id}>Home</div>
  )
}

export default Home