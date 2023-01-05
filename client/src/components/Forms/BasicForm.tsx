import React, {
  FormEvent,
  HTMLProps,
  ReactElement,
  useCallback,
  useState
} from "react";
import { useTranslation } from "react-i18next";
import Button from "../BasicButton/Button";
import style from "./BasicForm.scss";
import { Field, FormAggregateObject, FormUnit, HttpMethod } from "./FormTypes";

type Props = {
  /**
   * The action URL of form
   */
  action: string;
  /**
   * children nodes of form
   */
  children: ReactElement;
  /**
   * Fields of form
   */
  fields: Field[];
  /**
   * HTML properties of form
   */
  formProperties?: HTMLProps<HTMLFormElement>;
  /**
   * id of form
   */
  id: string;
  /**
   * HTTP method of form submit
   */
  method: HttpMethod;
  /**
   * title of submit button
   */
  submitTitle: string;
};

/**
 * onSubmit function
 * @param event the on submit form event
 * @param pendingUpdater the pending updater to remove "loading" message
 * @param resultUpdater the result updater to display errors if necessary
 * @param fields fields in form html elements
 * @param extraData aditional datas of form to send when submit button is pressed
 */
const onSubmitRequest = async (
  event: FormEvent<HTMLFormElement>,
  pendingUpdater: (state: boolean) => void,
  resultUpdater: (success: boolean, error?: string) => void,
  fields: Field[],
  extraData?: FormAggregateObject
): Promise<void> => {
  event.preventDefault();
  const formElements = event.currentTarget.elements;

  const formData = new Map<string, FormUnit>(
    fields.map((field: Field) => {
      const fieldInput = formElements.namedItem(field.name);
      let value = null;
      if (
        fieldInput instanceof HTMLInputElement ||
        fieldInput instanceof RadioNodeList ||
        fieldInput instanceof HTMLTextAreaElement ||
        fieldInput instanceof HTMLSelectElement
      ) {
        value = fieldInput.value;
      }
      return [field.name, value];
    })
  );

  console.info("start spinner and lock button");
  pendingUpdater(true);
  try {
    const response = await fetch(
      event.currentTarget.getAttribute("action") || "/",
      {
        method: event.currentTarget.getAttribute("method") || "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify({ ...Object.fromEntries(formData), ...extraData })
      }
    );

    if (response.status <= 400) {
      console.info("display success notification");
      resultUpdater(true);
    } else {
      console.error("display error notification with code", response.status);
      resultUpdater(false, `Error notification with code ${response.status}`);
    }
  } catch (error) {
    console.error("display error notification", error);
    const message = error instanceof Error ? error.message : "unknown error";
    resultUpdater(false, `Error notification ${message}`);
  }
  console.info("end spinner and enable button");
  pendingUpdater(false);
};

function BasicForm({
  action,
  children,
  fields,
  formProperties,
  id,
  method,
  submitTitle
}: Props) {
  const { t } = useTranslation();
  const [pendingState, setPendingState] = useState(false);
  const [errors, setErrors] = useState<Array<string>>([]);
  const onSubmit = useCallback(
    (event: FormEvent<HTMLFormElement>) => {
      onSubmitRequest(
        event,
        setPendingState,
        (success: boolean, error?: string) => {
          if (success) {
            console.log("success");
          }
          if (error) {
            setErrors([...errors, error]);
          }
        },
        fields
      );
    },
    [errors, fields]
  );
  return (
    <>
      <form
        method={method}
        action={action}
        id={id}
        onSubmit={onSubmit}
        className={style.BasicForm}
        {...formProperties}
      >
        {children}

        <Button
          id={`submit-${id}`}
          level="primary"
          disabled={pendingState}
          type="submit"
          value={String(t(submitTitle))}
        >
          {String(t(submitTitle))}
        </Button>
      </form>
      {pendingState ? <div>{String(t("loading"))}</div> : null}
    </>
  );
}

export default BasicForm;
