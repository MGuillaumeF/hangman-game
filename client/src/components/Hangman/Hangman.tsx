import React from "react";

type Props = {
  height: number;
  quantity: number;
  width: number;
};

function Path({ children, ...props }: React.SVGProps<SVGPathElement>) {
  return <path {...props}>{children}</path>;
}
function Circle({ children, ...props }: React.SVGProps<SVGCircleElement>) {
  return <circle {...props}>{children}</circle>;
}

interface IPartProps extends React.SVGProps<SVGElement> {
  Type: Function;
}

const parts: IPartProps[] = [
  {
    Type: Path,
    d: "M0 487.5 L125 487.5",
    stroke: "#000000",
    strokeWidth: "25"
  },
  {
    Type: Path,
    d: "M37.5 475 L37.5 10",
    stroke: "#000000",
    strokeWidth: "25"
  },
  {
    Type: Path,
    d: "M37.5 22.5 L245 22.5",
    stroke: "#000000",
    strokeWidth: "25"
  },
  {
    Type: Path,
    d: "M40 97.5 L122.5 25",
    stroke: "#000000",
    strokeWidth: "18"
  },
  {
    Type: Path,
    d: "M247.5 10 L247.5 75",
    stroke: "#000000",
    strokeWidth: "6"
  },
  {
    Type: Circle,
    cx: "247.5",
    cy: "125",
    r: "50",
    fill: "transparent",
    stroke: "#000000",
    strokeWidth: "4"
  },
  {
    Type: Path,
    d: "M247.5 175 L247.5 300",
    stroke: "#000000",
    strokeWidth: "4"
  },
  {
    Type: Path,
    d: "M247.5 300 L300.5 335",
    stroke: "#000000",
    strokeWidth: "4"
  },
  {
    Type: Path,
    d: "M247.5 300 L194.5 335",
    stroke: "#000000",
    strokeWidth: "4"
  },
  {
    Type: Path,
    d: "M247.5 220 L300.5 255",
    stroke: "#000000",
    strokeWidth: "4"
  },
  {
    Type: Path,
    d: "M247.5 220 L194.5 255",
    stroke: "#000000",
    strokeWidth: "4"
  }
];

function Hangman({ width, height, quantity }: Props) {
  return (
    <svg
      version="1.1"
      viewBox="0 0 500 500"
      width={String(width)}
      height={String(height)}
      xmlns="http://www.w3.org/2000/svg"
    >
      {parts.slice(0, quantity).map((value: IPartProps, index: number) => {
        const { Type, ...props } = value;
        if (index === quantity - 1) {
          return (
            <Type key={index} {...props} strokeDasharray="2000">
              <animate
                attributeName="strokeDashoffset"
                values="2037;0;2037"
                begin="2s"
                dur="5"
                calcMode="spline"
              />
            </Type>
          );
        } else {
          return <Type key={index} {...props} />;
        }
      })}
      <path
        d="M0 487.5 L125 487.5"
        stroke="#000000"
        strokeWidth="25"
        strokeDasharray="2000"
      >
        <animate
          attributeName="strokeDashoffset"
          values="2037;0;2037"
          dur="5"
          calcMode="spline"
        />
      </path>
    </svg>
  );
}

export default Hangman;
