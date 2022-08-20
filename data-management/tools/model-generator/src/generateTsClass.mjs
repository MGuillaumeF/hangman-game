import { load } from "loader";

export function generateTsClass(modelClasses) {
  console.log(load("TsClasses", {
  dependencies : "",
  className: "",
  extendedClasses : "",
  privateAttributes : "",
  privateStaticAttributes:"",
  privateMethods:"",
  privateStaticMethods : "",
  protectedAttributes : "",
  protectedStaticAttributes : "",
  protectedMethods : "",
  protectedStaticMethods : "",
  publicAttributes : "",
  publicStaticAttributes : "",
  publicMethods : "",
  publicStaticMethods : ""
}));
}
